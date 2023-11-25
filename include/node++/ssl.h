#ifndef NODEPP_SSL
#define NODEPP_SSL

/*────────────────────────────────────────────────────────────────────────────*/

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "fs.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/
namespace _ssl_ { struct str {
    int        tpy = SSL_FILETYPE_PEM;
    SSL_CTX*   ctx = nullptr;
    SSL*       ssl = nullptr;
    string_t   key, cert;
    bool       srv = 0;
};}

/*────────────────────────────────────────────────────────────────────────────*/

class ssl_t { 
protected:
    
    using onSNI = function_t<ssl_t*,string_t>;
    ptr_t<_ssl_::str> obj = new _ssl_::str();
    ptr_t<onSNI> func;
    
    /*─······································································─*/

    SSL_CTX* create_server_context() const noexcept {
        const SSL_METHOD *method; method = TLS_server_method();
        SSL_CTX* ctx = SSL_CTX_new( method );
        SSL_CTX_set_timeout( ctx, 0 );
        return ctx;
    }
    
    /*─······································································─*/

    SSL_CTX* create_client_context() const noexcept {
        const SSL_METHOD *method; method = TLS_client_method();
        SSL_CTX* ctx = SSL_CTX_new( method ); 
        SSL_CTX_set_timeout( ctx, 0 );
        return ctx;
    }
    
    /*─······································································─*/

    int configure_context( SSL_CTX* ctx, string_t key, string_t cert ) const noexcept { return (  
        SSL_CTX_use_certificate_file( ctx, (char*)cert, obj->tpy ) != 1 ||
        SSL_CTX_use_PrivateKey_file( ctx, (char*)key, obj->tpy )   != 1
    ) ? -1 : 1; }
    
    /*─······································································─*/

    static int servername_handler( SSL* ssl, int* ao, void* arg ) {
        const char* servername = SSL_get_servername( ssl, TLSEXT_NAMETYPE_host_name );
        onSNI func = *((onSNI*)arg); if( servername ){ ssl_t* xtc = func(servername); 
            if( xtc != nullptr ){ xtc->create_server();
                SSL_CTX* v = SSL_set_SSL_CTX( ssl, xtc->get_ctx() );
                if( v != xtc->get_ctx() ) return SSL_TLSEXT_ERR_NOACK;
            }
        }   return SSL_TLSEXT_ERR_OK;
    }
    
    /*─······································································─*/

    bool is_blocked( const int& c ) const noexcept { if( c<0 ){
        int error = SSL_get_error( obj->ssl, c ); return ( 
            error == SSL_ERROR_WANT_CLIENT_HELLO_CB ||
            error == SSL_ERROR_WANT_X509_LOOKUP     ||
            error == SSL_ERROR_WANT_ASYNC_JOB       ||
            error == SSL_ERROR_WANT_CONNECT         ||
            error == SSL_ERROR_WANT_ACCEPT          ||
            error == SSL_ERROR_WANT_ASYNC           ||
            error == SSL_ERROR_WANT_WRITE           ||
            error == SSL_ERROR_WANT_READ
        );
    } return 0; }

    /*─······································································─*/

    bool is_error( const int& c ) const noexcept { if( c < 0 ){
        int error =  SSL_get_error( obj->ssl, c ); return ( 
            error == SSL_ERROR_ZERO_RETURN          ||
            error == SSL_ERROR_SYSCALL              ||
            error == SSL_ERROR_SSL
        );
    } return 0; }
    
    /*─······································································─*/

    void set_ctx_sni( SSL_CTX* ctx, onSNI* func ) const noexcept {
        SSL_CTX_set_tlsext_servername_callback( ctx, servername_handler );
        SSL_CTX_set_tlsext_servername_arg( ctx, func );
    }
    
    /*─······································································─*/

    void set_nonbloking_mode() const noexcept {
        SSL_set_mode( obj->ssl, SSL_MODE_ASYNC | SSL_MODE_AUTO_RETRY | SSL_MODE_RELEASE_BUFFERS );
    }

public: 
    
    virtual ~ssl_t() {
        if( obj.count() > 1 ) { return; }
            force_close();
    }

    void force_close() const noexcept {
        if( obj->ssl != nullptr ){
            if( obj->srv==1 ){ SSL_shutdown(obj->ssl); } 
            SSL_free(obj->ssl); obj->ssl = nullptr;
            return;
        }
        if( obj->ctx != nullptr ){
            SSL_CTX_free(obj->ctx); 
            obj->ctx = nullptr; 
            return;
        }
    }

    void free() const noexcept { force_close(); } 
    
    /*─······································································─*/

    ssl_t( string_t _key, string_t _cert, onSNI* _func=nullptr ){
        if( !fs::exists_file(_key) || !fs::exists_file(_cert) )
            _Error("such key or cert does not exist");
        if( _func != nullptr ) func = new onSNI(*_func); 
             obj->key = _key; obj->cert = _cert; 
    }

    ssl_t( string_t _key, string_t _cert, onSNI _func ){
        this->ssl_t( _key, _cert, &_func );
    }

    ssl_t() noexcept = default;

    /*─······································································─*/

    ssl_t( ssl_t* xtc, int df ){
        if( xtc == nullptr ) _Error("ctx is nullptr");
        if( xtc->get_ctx() == nullptr ) _Error("ctx has no context");
        obj->ctx=xtc->get_ctx(); obj->ssl=SSL_new(obj->ctx); obj->srv=xtc->is_server();
        if( obj->ctx == nullptr ) _Error("can't start SSL"); set_nonbloking_mode(); set_fd( df );
    }
    
    /*─······································································─*/

    void set_ctx_type( int type ) const noexcept { obj->tpy = type; }
    SSL_CTX*  get_ctx() const noexcept { return obj->ctx; }
    SSL*      get_ssl() const noexcept { return obj->ssl; }
    bool    is_server() const noexcept { return obj->srv; }
    
    /*─······································································─*/

    int create_server() const noexcept {
        obj->ctx = create_server_context(); obj->srv = 1;
        int res = configure_context( obj->ctx, obj->key, obj->cert ); 
        if( func != nullptr ){ set_ctx_sni( obj->ctx, &func ); } return res;
    }
    
    /*─······································································─*/

    int create_client() const noexcept {
        obj->ctx = create_client_context(); obj->srv = 0; 
        return configure_context( obj->ctx, obj->key, obj->cert );
    }
    
    /*─······································································─*/

    int set_fd( int df ) noexcept { return obj->ssl==nullptr ? -1 : SSL_set_fd( obj->ssl, df ); }
    int  get_rfd() const noexcept { return obj->ssl==nullptr ? -1 : SSL_get_rfd( obj->ssl ); }
    int  get_wfd() const noexcept { return obj->ssl==nullptr ? -1 : SSL_get_wfd( obj->ssl ); }
    int   get_fd() const noexcept { return obj->ssl==nullptr ? -1 : SSL_get_fd( obj->ssl ); }
    
    /*─······································································─*/

    int _accept() const noexcept {
        if( obj->ssl == nullptr ){ return -1; } int c = 0;
        return is_blocked( c=SSL_accept(obj->ssl) ) ? -2 : c;
    }
    
    /*─······································································─*/

    int _connect() const noexcept { 
        if( obj->ssl == nullptr ){ return -1; } int c = 0;
        return is_blocked( c=SSL_connect(obj->ssl) ) ? -2 : c;
    }
    
    /*─······································································─*/

    int accept() const noexcept { int c = 0;
        while(( c=_accept() )==-2 ){ process::next(); } return c;
    }
    
    /*─······································································─*/

    int connect() const noexcept { int c = 0;
        while(( c=_connect() )==-2 ){ process::next(); } return c;
    }
    
    /*─······································································─*/

    ulong read( char* buffer, const ulong& size ) const noexcept { int c = 0;
        while(( c=_read(buffer,size) )==-2 ){ process::next(); } return c;
    }
    
    ulong write( char* buffer, const ulong& size ) const noexcept { int c = 0;
        while(( c=_write(buffer,size) )==-2 ){ process::next(); } return c;
    }
    
    /*─······································································─*/

    int _read( char* bf, ulong sx ) const noexcept {
        if( obj->ssl == nullptr ){ return -1; } int c = 0;
        return is_blocked( c=SSL_read( obj->ssl, bf, sx ) ) ? -2 : c;
    }
    
    int _write( char* bf, ulong sx ) const noexcept {
        if( obj->ssl == nullptr ){ return -1; } int c = 0;
        return is_blocked( c=SSL_write( obj->ssl, bf, sx ) ) ? -2 : c;
    }
    
};

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
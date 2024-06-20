/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_CRYPTO
#define NODEPP_CRYPTO
#define OPENSSL_API_COMPAT 0x10100000L

/*────────────────────────────────────────────────────────────────────────────*/

#include <openssl/ripemd.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/des.h>

/*────────────────────────────────────────────────────────────────────────────*/

#include <openssl/dh.h>
#include <openssl/dsa.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

/*────────────────────────────────────────────────────────────────────────────*/

#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/ecdh.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_PCB
#define NODEPP_PCB
namespace { int pcb ( char *buf, int size, int rwflag, void *args ) {
    if( args == nullptr ){ return 0; }
    strncpy( buf, (char *)args, size );
             buf[ size - 1 ] = '\0';
    return strlen(buf);
}}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace crypto {

    void start_device(){ static bool ssl=false; 
        if( ssl == false ){
            OpenSSL_add_all_algorithms();
        }   ssl = true;
    }

    string_t buff2hex( const string_t& inp ){
        string_t out; for( auto x : inp ){
            out += string::format( "%02x", (uchar)x );
        }   return out;
    }

    string_t hex2buff( const string_t& inp ){
        auto x = inp; string_t out; while( !x.empty() ){
            auto y = x.splice(0,2); char ch=0;
            string::parse(y,"%02x",&ch);
            out.push( ch );
        }   return out;
    }

    string_t genkey( const string_t& alph, int x=32 ) { 
        string_t data ( (ulong)x, '\0' ); while( x --> 0 ){
            data[x] = alph[ rand() % ( alph.size() - 1 ) ];
        }   return data;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

class hash_t {
protected:

    struct NODE {
        ptr_t<uchar> buff;
        EVP_MD_CTX* ctx;
        uint length;
        int  state;
    };  ptr_t<NODE> obj = new NODE();

    string_t hex() const noexcept { 
        free(); return { (char*) &obj->buff, obj->length }; 
    }

public:

    template< class T >
    hash_t( const T& type, ulong length ) 
    :  obj( new NODE() ) { crypto::start_device();
        obj->buff  = ptr_t<uchar>( length );
        obj->ctx   = EVP_MD_CTX_new();
        obj->state = 1;
        if ( !obj->ctx || !EVP_DigestInit_ex( obj->ctx, type, NULL ) )
           { process::error("can't initializate hash_t"); }
    }

    virtual ~hash_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    void update( const string_t& msg ) const noexcept { 
        if( obj->state != 1 ){ return; }
        EVP_DigestUpdate( obj->ctx, (uchar*) msg.data(), msg.size() );
    }

    void free() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_DigestFinal_ex( obj->ctx, &obj->buff, &obj->length );
        EVP_MD_CTX_free( obj->ctx ); //EVP_cleanup();
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    string_t get() const noexcept { 
        return crypto::buff2hex( this->hex() );
    }

    void close() const noexcept { free(); } 

};

/*────────────────────────────────────────────────────────────────────────────*/

class hmac_t {
protected:

    struct NODE {
        ptr_t<uchar> buff;
        HMAC_CTX* ctx; 
        uint length;
        int  state;
    };  ptr_t<NODE> obj;

    string_t hex() const noexcept { 
        free(); return { (char*) &obj->buff, obj->length }; 
    }

public:

    template< class T >
    hmac_t( const string_t& key, const T& type, ulong length ) 
    :  obj( new NODE() ) { crypto::start_device();
        obj->buff  = ptr_t<uchar>( length ); 
        obj->ctx   = HMAC_CTX_new(); 
        obj->state = 1;
        if ( !obj->ctx || !HMAC_Init_ex( obj->ctx, key.data(), key.size(), type, nullptr ) )
           { process::error("can't initializate hmac_t"); }
    }
    
    virtual ~hmac_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    void update( const string_t& msg ) const noexcept { 
        if( obj->state != 1 ){ return; }
        HMAC_Update( obj->ctx, (uchar*) msg.data(), msg.size() ); 
    }

    void free() const noexcept {
        if( obj->state == 0 ){ return; } obj->state = 0;
        HMAC_Final( obj->ctx, &obj->buff, &obj->length ); 
        HMAC_CTX_free( obj->ctx ); //EVP_cleanup();
    }

    string_t get() const noexcept { 
        return crypto::buff2hex( this->hex() );
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { free(); } 

};

/*────────────────────────────────────────────────────────────────────────────*/

class encrypt_t {
protected:

    struct NODE {
        EVP_CIPHER_CTX* ctx;
        ptr_t<uchar> bff;
        string_t buff;
        int state, len;
    };  ptr_t<NODE> obj;

    string_t hex() const noexcept { free(); return obj->buff; }

public:

    event_t<string_t> onData;
    event_t<>         onClose;

    template< class T >
    encrypt_t( const string_t& iv, const string_t& key, const T& type ) 
    :     obj( new NODE() ) { crypto::start_device();
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx   =    EVP_CIPHER_CTX_new(); 
        obj->state = 1; EVP_CIPHER_CTX_init( obj->ctx ); 
        if ( !obj->ctx || !EVP_EncryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), (uchar*)iv.data() ) )
           { process::error("can't initializate encrypt_t"); }
    }

    template< class T >
    encrypt_t( const string_t& key, const T& type ) 
    :     obj( new NODE() ) { crypto::start_device();
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx   =       EVP_CIPHER_CTX_new(); 
        obj->state = 1;    EVP_CIPHER_CTX_init( obj->ctx );
        if ( !obj->ctx || !EVP_EncryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), (uchar*)"\0" ) )
           { process::error("can't initializate encrypt_t"); }
    }

    template< class T >
    encrypt_t( const T& type ) 
    :     obj( new NODE() ) { crypto::start_device();
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx   =       EVP_CIPHER_CTX_new(); 
        obj->state = 1;    EVP_CIPHER_CTX_init( obj->ctx );
        if ( !obj->ctx || !EVP_EncryptInit_ex( obj->ctx, type, NULL, (uchar*)"\0", (uchar*)"\0" ) )
           { process::error("can't initializate encrypt_t"); }
    }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_EncryptUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size() );
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); }}
    }
    
    virtual ~encrypt_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    string_t get() const noexcept { return crypto::buff2hex( this->hex() ); }

    void free() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_EncryptFinal( obj->ctx, &obj->bff, &obj->len );
        EVP_CIPHER_CTX_free( obj->ctx ); //EVP_cleanup();
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); } onClose.emit(); }
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { free(); } 

};

/*────────────────────────────────────────────────────────────────────────────*/

class decrypt_t {
protected:

    struct NODE {
        EVP_CIPHER_CTX* ctx; 
        ptr_t<uchar> bff;
        int state, len;
        string_t buff;
    };  ptr_t<NODE> obj;
    
public:

    event_t<string_t> onData;
    event_t<>         onClose;

    template< class T >
    decrypt_t( const string_t& iv, const string_t& key, const T& type ) 
    :     obj( new NODE() ) { crypto::start_device();
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx   =    EVP_CIPHER_CTX_new(); 
        obj->state = 1; EVP_CIPHER_CTX_init( obj->ctx );
        if ( !obj->ctx || !EVP_DecryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), (uchar*)iv.data() ) )
           { process::error("can't initializate decrypt_t"); }
    }

    template< class T >
    decrypt_t( const string_t& key, const T& type ) 
    :     obj( new NODE() ) { crypto::start_device();
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx   =    EVP_CIPHER_CTX_new(); 
        obj->state = 1; EVP_CIPHER_CTX_init( obj->ctx );
        if ( !obj->ctx || !EVP_DecryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), (uchar*)"\0" ) )
           { process::error("can't initializate decrypt_t"); }
    }

    template< class T >
    decrypt_t( const T& type ) 
    :     obj( new NODE() ) { crypto::start_device();
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx   =    EVP_CIPHER_CTX_new(); 
        obj->state = 1; EVP_CIPHER_CTX_init( obj->ctx );
        if ( !obj->ctx || !EVP_DecryptInit_ex( obj->ctx, type, NULL, (uchar*)"\0", (uchar*)"\0" ) )
           { process::error("can't initializate decrypt_t"); }
    }

    void update( const string_t& msg ) const noexcept { 
        if( obj->state != 1 ){ return; } auto data = crypto::hex2buff( msg );
        EVP_DecryptUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)data.data(), data.size());
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); }}
    }
    
    virtual ~decrypt_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    string_t get() const noexcept { free(); return obj->buff; }

    void free() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_DecryptFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_CIPHER_CTX_free( obj->ctx ); //EVP_cleanup();
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); } onClose.emit(); }
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { free(); } 

};

/*────────────────────────────────────────────────────────────────────────────*/

class enc_base64_t {
protected:

    struct NODE {
        EVP_ENCODE_CTX* ctx; 
        ptr_t<uchar> bff;
        int state, len;
        string_t buff;
    };  ptr_t<NODE> obj;
    
public:

    event_t<string_t> onData;
    event_t<>         onClose;

    enc_base64_t() 
    :        obj( new NODE() ) { crypto::start_device();
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,0);
        obj->ctx   = EVP_ENCODE_CTX_new();
        obj->state = 1;
        if ( !obj->ctx )
           { process::error("can't initializate base64 encoder"); }
        EVP_EncodeInit( obj->ctx );
    }
    
    virtual ~enc_base64_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_EncodeUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size()); 
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); }}
    }

    string_t get() const noexcept { free(); return obj->buff; }

    void free() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_EncodeFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_ENCODE_CTX_free( obj->ctx ); //EVP_cleanup();
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); } onClose.emit(); }
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { free(); } 

};

/*────────────────────────────────────────────────────────────────────────────*/

class encoder_t {
protected:

    struct NODE {
        BIGNUM* bn;
        string_t chr;
        string_t buff;
        int      state;
    };  ptr_t<NODE> obj;

public:

    encoder_t( const string_t& chr ) 
    :     obj( new NODE() ) { crypto::start_device();
        obj->state = 1; obj->chr = chr; 
        obj->bn = (BIGNUM*) BN_new();
        if ( !obj->bn )
           { process::error("can't initializate encoder"); }
    }
    
    virtual ~encoder_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    string_t get() const noexcept { free(); return obj->buff; }

    void update( const string_t& msg ) const noexcept { 
        if( obj->state != 1 ){ return; }

        BN_bin2bn( (uchar*)msg.data(), msg.size(), obj->bn );

        while( BN_cmp( obj->bn, BN_value_one() ) > 0 ) {
            int rem = BN_div_word( obj->bn, obj->chr.size() );
                obj->buff.unshift( obj->chr[rem] );
        }

        for( const auto& byte : msg ) {
            if( byte != 0x00 ){ break; }
                obj->buff.unshift( obj->chr[0] );
        }

    }

    void free() const noexcept { 
        if( obj->state == 0 ){ return; }
            obj->state  = 0;
        BN_clear_free( obj->bn );
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { free(); }

};

/*────────────────────────────────────────────────────────────────────────────*/

class dec_base64_t {
protected:

    struct NODE {
        EVP_ENCODE_CTX* ctx; 
        ptr_t<uchar> bff;
        int state, len;
        string_t buff;
    };  ptr_t<NODE> obj;

public:

    event_t<string_t> onData;
    event_t<>         onClose;

    dec_base64_t() 
    :        obj( new NODE() ) { crypto::start_device();
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,0);
        obj->ctx   = EVP_ENCODE_CTX_new();
        obj->state = 1; 
        if ( !obj->ctx )
           { process::error("can't initializate base64 decoder"); }
        EVP_DecodeInit( obj->ctx );
    }
    
    virtual ~dec_base64_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_DecodeUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size()); 
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); }}
    }

    string_t get() const noexcept { free(); return obj->buff; }

    void free() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_DecodeFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_ENCODE_CTX_free( obj->ctx ); //EVP_cleanup();
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); } onClose.emit(); }
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { free(); } 

};

/*────────────────────────────────────────────────────────────────────────────*/

class decoder_t {
protected:

    struct NODE {
        BIGNUM* bn;
        string_t chr;
        string_t buff;
        int      state;
    };  ptr_t<NODE> obj;

public:

    event_t<string_t> onData;
    event_t<>         onClose;

    decoder_t( const string_t& chr ) 
    :     obj( new NODE() ) { crypto::start_device();
        obj->state = 1; obj->chr = chr; 
        obj->bn = (BIGNUM*) BN_new();
        if ( !obj->bn )
           { process::error("can't initializate decoder"); }
    }
    
    virtual ~decoder_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    void update( const string_t& msg ) const { 
        if( obj->state != 1 ){ return; }

        for( const auto& c : msg ) {
             const char* pos = strchr( obj->chr.data(), c );
             if( pos == nullptr ) process::error("Invalid BaseX character");
             BN_mul_word( obj->bn, obj->chr.size() );
             BN_add_word( obj->bn, pos - obj->chr.data() );
        }

        ptr_t<uchar> out ( BN_num_bytes(obj->bn) );
        BN_bn2bin( obj->bn, out.data() ); if ( onData.empty() ) {
                 obj->buff += string_t( (char*) &out, out.size() );
        } else { onData.emit( string_t( (char*) &out, out.size() ) ); }
    }

    string_t get() const noexcept { free(); return obj->buff; }

    void free() const noexcept { 
        if( obj->state == 1 ){ return; } obj->state = 0;
         BN_clear_free( obj->bn ); onClose.emit();
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { free(); } 

};

/*────────────────────────────────────────────────────────────────────────────*/

class ec_t {
protected:

    struct NODE {
        EC_GROUP *key_group = nullptr;
        EC_POINT *pub_key   = nullptr;
        BIGNUM   *priv_key  = nullptr;
        EC_KEY   *key_pair  = nullptr;
        int       state;
    };  ptr_t<NODE> obj;
    
public:

    template< class T >
    ec_t( const string_t& key, const T& type ) noexcept 
    :   obj( new NODE() ) { crypto::start_device();
        obj->state = 1;

        obj->key_pair  = EC_KEY_new_by_curve_name(type);
        obj->key_group = EC_GROUP_new_by_curve_name(type);

        obj->priv_key = (BIGNUM*) BN_new(); 
        BN_hex2bn( &obj->priv_key, key.data() );
        EC_KEY_set_private_key( obj->key_pair, obj->priv_key );

        obj->pub_key = (EC_POINT*) EC_POINT_new( obj->key_group );
        EC_POINT_mul( obj->key_group, obj->pub_key, obj->priv_key, nullptr, nullptr, nullptr );
        EC_KEY_set_public_key( obj->key_pair, obj->pub_key );

    }

    template< class T >
    ec_t( const T& type ) noexcept 
    :   obj( new NODE() ) { crypto::start_device();
        obj->state = 1;

        obj->key_pair  = EC_KEY_new();
        obj->key_group = EC_GROUP_new_by_curve_name( type );

        EC_KEY_set_group( obj->key_pair, obj->key_group );
        EC_KEY_generate_key( obj->key_pair );

        obj->pub_key  = (EC_POINT*) EC_KEY_get0_public_key( obj->key_pair );
        obj->priv_key = (BIGNUM*)  EC_KEY_get0_private_key( obj->key_pair );
    }
    
    virtual ~ec_t() noexcept { if( obj.count()>1 ){ return; } free(); }

    string_t get_public_key( uint x = 0 ) const noexcept { 
        if( obj->state != 1 ){ return nullptr; }
        point_conversion_form_t y; switch( x ){
            case 0:  y = POINT_CONVERSION_HYBRID;       break;
            case 1:  y = POINT_CONVERSION_COMPRESSED;   break;
            default: y = POINT_CONVERSION_UNCOMPRESSED; break;
        }   return EC_POINT_point2hex( obj->key_group, obj->pub_key, y, nullptr );
    }

    string_t get_private_key() const noexcept { return BN_bn2hex( obj->priv_key ); }

    void free() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        if( obj->priv_key  != nullptr ) BN_free( obj->priv_key );
    //  if( obj->key_pair  != nullptr ) EC_KEY_free( obj->key_pair );
        if( obj->pub_key   != nullptr ) EC_POINT_free( obj->pub_key );
        if( obj->key_group != nullptr ) EC_GROUP_free( obj->key_group );
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { free(); } 

};

/*────────────────────────────────────────────────────────────────────────────*/

class rsa_t {
protected:

    struct NODE {
        RSA*    rsa = nullptr;
        BIGNUM* num = nullptr;
        ulong   len;
        int   state;
    };  ptr_t<NODE> obj;
    
public:

    rsa_t( int keylen=2048 ) : obj( new NODE() ) {
        crypto::start_device();
        obj->rsa   = RSA_new();
        obj->num   =  BN_new();
        obj->len   = keylen; 
        obj->state = 1;
        if ( !obj->num || !obj->rsa )
           { process::error("creating rsa object"); }
        BN_set_word( obj->num, RSA_F4 );
        RSA_generate_key_ex( obj->rsa, keylen, obj->num, NULL );
    }

    virtual ~rsa_t() noexcept { if( obj.count() > 1 ){ return; } free(); }

    int write_private_key( const string_t& path, const char* pass=NULL ) const {
        FILE* fp = fopen( path.data() , "w"); int res = 0;
        if ( fp == nullptr ){ process::error("while writing private key"); }
        res = PEM_write_RSAPrivateKey( fp, obj->rsa, NULL, NULL, 0, pcb, (void*)pass );
        fclose( fp ); return res;
    }

    int write_public_key( const string_t& path ) const {
        FILE* fp = fopen( path.data() , "w"); int res = 0;
        if ( fp == nullptr ){ process::error("while writing public key"); }
        res = PEM_write_RSAPublicKey( fp, obj->rsa );
        fclose( fp ); return res;
    }

    void read_public_key( const string_t& path, const char* pass=NULL ) const {
        FILE* fp = fopen( path.data(), "r" );
        if ( fp == nullptr ){ process::error("while reading public key"); }
        PEM_read_RSAPublicKey( fp, &obj->rsa, pcb, (void*)pass ); 
        fclose( fp );
    }

    void read_private_key( const string_t& path, const char* pass=NULL ) const {
        FILE* fp = fopen( path.data(), "r" );
        if ( fp == nullptr ){ process::error("while reading private key"); }
        PEM_read_RSAPrivateKey( fp, &obj->rsa, pcb, (void*)pass ); 
        fclose( fp );
    }

    string_t public_encrypt( const string_t& msg, int padding=RSA_PKCS1_PADDING ) const {
        if ( msg.empty() || obj->state ==0 ){ return nullptr; }
        ptr_t<uchar> out ( RSA_size( obj->rsa ), '\0' );
        int c = RSA_public_encrypt( msg.size(), (uchar*)msg.data(), &out, obj->rsa, padding );
        return crypto::buff2hex( string_t( (char*)& out, (ulong)c ) );
    }

    string_t private_encrypt( const string_t& msg, int padding=RSA_PKCS1_PADDING ) const {
        if( msg.empty() || obj->state ==0 ){ return nullptr; }
        ptr_t<uchar> out ( RSA_size( obj->rsa ), '\0' );
        int c = RSA_private_encrypt( msg.size(), (uchar*)msg.data(), &out, obj->rsa, padding );
        return crypto::buff2hex( string_t( (char*)& out, (ulong)c ) );
    }

    string_t public_decrypt( const string_t& msg, int padding=RSA_PKCS1_PADDING ) const {
        if( msg.empty() || obj->state == 0 ){ return nullptr; }
        ptr_t<uchar> out ( RSA_size( obj->rsa ), '\0' ); auto gsm = crypto::hex2buff( msg );
        int c = RSA_public_decrypt( gsm.size(), (uchar*)gsm.data(), &out, obj->rsa, padding );
        return string_t( (char*)& out, (ulong)c );
    }

    string_t private_decrypt( const string_t& msg, int padding=RSA_PKCS1_PADDING ) const {
        if( msg.empty() || obj->state == 0 ){ return nullptr; }
        ptr_t<uchar> out ( RSA_size( obj->rsa ), '\0' ); auto gsm = crypto::hex2buff( msg );
        int c = RSA_private_decrypt( gsm.size(), (uchar*)gsm.data(), &out, obj->rsa, padding );
        return string_t( (char*)& out, (ulong)c );
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { free(); } 

    void free() const noexcept { 
        if( obj.count() > 1 ){ return; } 
        if( obj->state == 0 ){ return; } obj->state =0;
        if( obj->rsa != nullptr ) RSA_free( obj->rsa );
        if( obj->num != nullptr )  BN_free( obj->num );
    }
    
};

/*────────────────────────────────────────────────────────────────────────────*/

class dh_t {
protected:

    struct NODE {
        DH* dh;
        int state;
        BIGNUM *k;
    };  ptr_t<NODE> obj;

public:

    dh_t( int len=512 ) : obj( new NODE() ) {
        crypto::start_device();
        obj->dh    = DH_new(); 
        obj->k     = BN_new();
        obj->state = 1;
        if( !obj->dh || !obj->k )
          { process::error( "creating new dh" ); }
        if( !DH_generate_parameters_ex( obj->dh, len, DH_GENERATOR_2, NULL ) )
          { process::error( "while generating dh parameters" ); }
        if( !DH_generate_key( obj->dh ) )
          { process::error( "while generating dh key" ); }
    }

    virtual ~dh_t() noexcept { if( obj.count() > 1 ){ return; } free(); }

    int set_public_key( const string_t& key ) const noexcept {
        if( obj->state != 1 ){ return 0; }
               BN_hex2bn( &obj->k, key.data() );
        return DH_set0_key( obj->dh, nullptr, obj->k );
    }

    int set_private_key( const string_t& key ) const noexcept {
        if( obj->state != 1 ){ return 0; }
               BN_hex2bn( &obj->k, key.data() );
        return DH_set0_key( obj->dh, obj->k, nullptr );
    }

    string_t get_private_key() const noexcept {
        if( obj->state != 1 ){ return nullptr; } 
        return BN_bn2hex( DH_get0_priv_key( obj->dh ) );
    }

    string_t get_public_key() const noexcept {
        if( obj->state != 1 ){ return nullptr; } 
        return BN_bn2hex( DH_get0_pub_key( obj->dh ) );
    }

    void free() const noexcept {
        if( obj->state == 0 ){ return; } obj->state = 0;
        DH_free( obj->dh ); BN_free( obj->k );
    }

    string_t sign( const string_t& hex ) const noexcept {
        if( obj->state != 1 ){ return nullptr; } 
        ptr_t<uchar> shared( DH_size( obj->dh ) );
                  BN_hex2bn( &obj->k, hex.data() );
        int len = DH_compute_key( &shared, obj->k, obj->dh );
        return crypto::buff2hex( string_t( (char*) &shared, (ulong) len ) );
    }

    bool verify( const string_t& hex, const string_t& sgn ) const noexcept {
         return sign( hex ) == sgn;
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

class dsa_t {
protected:

    struct NODE {
        DSA    *dsa = nullptr;
        int     state;
        uint    len;
    };  ptr_t<NODE> obj;
    
public:

    dsa_t( uint size=512 ) 
    : obj( new NODE() ) { crypto::start_device();
        obj->state = 1; obj->len = size; obj->dsa = DSA_new(); 
        if(!DSA_generate_parameters_ex( obj->dsa, obj->len, NULL, 0, NULL, NULL, NULL ) )
          { process::error("while generating DSA parameters"); }
        if(!DSA_generate_key( obj->dsa ) )
          { process::error("while generating DSA key"); }
    }

    virtual ~dsa_t() noexcept { if( obj.count() > 1 ){ return; } free(); }

    bool verify( const string_t& msg, const string_t& sgn ) const noexcept { 
         if( obj->state != 1 ){ return false; } auto ngs = crypto::hex2buff( sgn ); 
         return DSA_verify( 0, (uchar*)msg.data(), msg.size(), (uchar*)ngs.data(), ngs.size(), obj->dsa )>0;
    }

    string_t sign( const string_t& msg ) const noexcept {
        if( obj->state != 1 ){ return nullptr; }
        ptr_t<uchar> sgn( DSA_size(obj->dsa) );  uint len;
        DSA_sign( 0, (uchar*)msg.data(), msg.size(), &sgn, &len, obj->dsa );
        return crypto::buff2hex( string_t( (char*) &sgn, (ulong) len ) );
    }

    void read_private_key( const string_t& path, const char* pass=NULL ) const {
        FILE* fp = fopen(path.data(),"r");
        if ( fp == nullptr ) process::error(" while reading private key");
        obj->dsa = PEM_read_DSAPrivateKey( fp, &obj->dsa, pcb, (void*)pass );
    }

    void read_public_key( const string_t& path, const char* pass=NULL ) const {
        FILE* fp = fopen(path.data(),"r");
        if ( fp == nullptr ) process::error(" while reading public key");
        obj->dsa = PEM_read_DSA_PUBKEY( fp, &obj->dsa, pcb, (void*)pass );
    }

    void write_private_key( const string_t& path ) const {
        if( obj->state != 1 ){ return; } FILE* fp = fopen( path.data(), "w" );
        if ( fp == nullptr ) { process::error("while creating file"); }
        if (!PEM_write_DSA_PUBKEY( fp, obj->dsa ) ) 
           { fclose( fp ); process::error("while writting the private key"); } fclose( fp );
    }

    void write_public_key( const string_t& path, const char* pass=NULL ) const {
        if( obj->state != 1 ){ return; } FILE* fp = fopen( path.data(), "w" );
        if ( fp == nullptr ) { process::error("while creating file"); }
        if (!PEM_write_DSAPrivateKey( fp, obj->dsa, nullptr, nullptr, 0, pcb, (void*)pass ) )
           { fclose( fp ); process::error("while writting the public key"); } fclose( fp );
    }

    void free() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        if( obj->dsa != nullptr ) DSA_free( obj->dsa );
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { free(); } 
    
};}

#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_CRYPTO) && defined(NODEPP_CRYPTO)
    #define  GENERATOR_CRYPTO
namespace nodepp {
namespace crypto { namespace hash {

    class MD4 : public hash_t { public:
          MD4() : hash_t( EVP_md4(), MD4_DIGEST_LENGTH ) {}
    };

    class MD5 : public hash_t { public:
          MD5() : hash_t( EVP_md5(), MD5_DIGEST_LENGTH ) {}
    };

    class SHA1 : public hash_t { public:
          SHA1() : hash_t( EVP_sha1(), SHA_DIGEST_LENGTH ) {}
    };

    class SHA256 : public hash_t { public:
          SHA256() : hash_t( EVP_sha256(), SHA256_DIGEST_LENGTH ) {}
    };

    class SHA384 : public hash_t { public:
          SHA384() : hash_t( EVP_sha384(), SHA384_DIGEST_LENGTH ) {}
    };

    class SHA512 : public hash_t { public:
          SHA512() : hash_t( EVP_sha512(), SHA512_DIGEST_LENGTH ) {}
    };

    class KECCAK256 : public hash_t { public:
          KECCAK256() : hash_t( EVP_sha3_256(), SHA256_DIGEST_LENGTH ) {}
    };

    class KECCAK384 : public hash_t { public:
          KECCAK384() : hash_t( EVP_sha3_384(), SHA384_DIGEST_LENGTH ) {}
    };

    class KECCAK512 : public hash_t { public:
          KECCAK512() : hash_t( EVP_sha3_512(), SHA512_DIGEST_LENGTH ) {}
    };

    class RIPEMD160 : public hash_t { public:
          RIPEMD160() : hash_t( EVP_ripemd160(), RIPEMD160_DIGEST_LENGTH ) {}
    }; 

}}
    
    /*─······································································─*/

namespace crypto { namespace hmac {

    class MD4 : public hmac_t { public:
          MD4 ( const string_t& key ) : hmac_t( key, EVP_md4(), MD4_DIGEST_LENGTH ) {}
    };

    class MD5 : public hmac_t { public:
          MD5 ( const string_t& key ) : hmac_t( key, EVP_md5(), MD5_DIGEST_LENGTH ) {}
    };

    class SHA1 : public hmac_t { public:
          SHA1 ( const string_t& key ) : hmac_t( key, EVP_sha1(), SHA_DIGEST_LENGTH ) {}
    };

    class SHA256 : public hmac_t { public:
          SHA256 ( const string_t& key ) : hmac_t( key, EVP_sha256(), SHA256_DIGEST_LENGTH ) {}
    };

    class SHA384 : public hmac_t { public:
          SHA384 ( const string_t& key ) : hmac_t( key, EVP_sha384(), SHA384_DIGEST_LENGTH ) {}
    };

    class SHA512 : public hmac_t { public:
          SHA512 ( const string_t& key ) : hmac_t( key, EVP_sha512(), SHA512_DIGEST_LENGTH ) {}
    };

    class KECCAK256 : public hmac_t { public:
          KECCAK256 ( const string_t& key ) : hmac_t( key, EVP_sha3_256(), SHA256_DIGEST_LENGTH ) {}
    };

    class KECCAK384 : public hmac_t { public:
          KECCAK384 ( const string_t& key ) : hmac_t( key, EVP_sha3_384(), SHA384_DIGEST_LENGTH ) {}
    };

    class KECCAK512 : public hmac_t { public:
          KECCAK512 ( const string_t& key ) : hmac_t( key, EVP_sha3_512(), SHA512_DIGEST_LENGTH ) {}
    };

    class RIPEMD160 : public hmac_t { public:
          RIPEMD160( const string_t& key ) : hmac_t( key, EVP_ripemd160(), RIPEMD160_DIGEST_LENGTH ) {}
    }; 

}}
    
    /*─······································································─*/

namespace crypto { namespace encrypt {
    
    class AES_128_CBC : public encrypt_t { public: template< class... T >
          AES_128_CBC( const T&... args ) : encrypt_t( args..., EVP_aes_128_cbc() ) {}
    };
    
    class AES_192_CBC : public encrypt_t { public: template< class... T >
          AES_192_CBC( const T&... args ) : encrypt_t( args..., EVP_aes_192_cbc() ) {}
    };
    
    class AES_256_CBC : public encrypt_t { public: template< class... T >
          AES_256_CBC( const T&... args ) : encrypt_t( args..., EVP_aes_256_cbc() ) {}
    };

    /*─······································································─*/
    
    class AES_128_ECB : public encrypt_t { public: template< class... T >
          AES_128_ECB( const T&... args ) : encrypt_t( args..., EVP_aes_128_ecb() ) {}
    };
    
    class AES_192_ECB : public encrypt_t { public: template< class... T >
          AES_192_ECB( const T&... args ) : encrypt_t( args..., EVP_aes_192_ecb() ) {}
    };
    
    class AES_256_ECB : public encrypt_t { public: template< class... T >
          AES_256_ECB( const T&... args ) : encrypt_t( args..., EVP_aes_256_ecb() ) {}
    };

    /*─······································································─*/
    
    class TRIPLE_DES_CFB : public encrypt_t { public: template< class... T >
          TRIPLE_DES_CFB ( const T&... args ) : encrypt_t( args..., EVP_des_ede3_cfb() ) {}
    };
    
    class TRIPLE_DES_CBC : public encrypt_t { public: template< class... T >
          TRIPLE_DES_CBC ( const T&... args ) : encrypt_t( args..., EVP_des_ede3_cbc() ) {}
    };
    
    class TRIPLE_DES_ECB : public encrypt_t { public: template< class... T >
          TRIPLE_DES_ECB ( const T&... args ) : encrypt_t( args..., EVP_des_ede3_ecb() ) {}
    };

    /*─······································································─*/
    
    class DES_EDE_CFB : public encrypt_t { public: template< class... T >
          DES_EDE_CFB ( const T&... args ) : encrypt_t( args..., EVP_des_ede_cfb() ) {}
    };
    
    class DES_EDE_CBC : public encrypt_t { public: template< class... T >
          DES_EDE_CBC ( const T&... args ) : encrypt_t( args..., EVP_des_ede_cbc() ) {}
    };
    
    class DES_EDE_ECB : public encrypt_t { public: template< class... T >
          DES_EDE_ECB ( const T&... args ) : encrypt_t( args..., EVP_des_ede_ecb() ) {}
    };

    /*─······································································─*/
    
    class DES_CFB : public encrypt_t { public: template< class... T >
          DES_CFB ( const T&... args ) : encrypt_t( args..., EVP_des_cfb() ) {}
    };
    
    class DES_CBC : public encrypt_t { public: template< class... T >
          DES_CBC ( const T&... args ) : encrypt_t( args..., EVP_des_cbc() ) {}
    };
    
    class DES_ECB : public encrypt_t { public: template< class... T >
          DES_ECB ( const T&... args ) : encrypt_t( args..., EVP_des_ecb() ) {}
    };

    /*─······································································─*/
    
    class RC4 : public encrypt_t { public: template< class... T >
          RC4 ( const T&... args ) : encrypt_t( args..., EVP_rc4() ) {}
    };

    /*─······································································─*/

    class RSA : public rsa_t { public: template< class... T > 
          RSA ( const T&... args ) : rsa_t( args... ) {}
    };

}}
    
    /*─······································································─*/

namespace crypto { namespace decrypt {
    
    class AES_128_CBC : public decrypt_t { public: template< class... T >
          AES_128_CBC( const T&... args ) : decrypt_t( args..., EVP_aes_128_cbc() ) {}
    };
    
    class AES_192_CBC : public decrypt_t { public: template< class... T >
          AES_192_CBC( const T&... args ) : decrypt_t( args..., EVP_aes_192_cbc() ) {}
    };
    
    class AES_256_CBC : public decrypt_t { public: template< class... T >
          AES_256_CBC( const T&... args ) : decrypt_t( args..., EVP_aes_256_cbc() ) {}
    };

    /*─······································································─*/
    
    class AES_128_ECB : public decrypt_t { public: template< class... T >
          AES_128_ECB( const T&... args ) : decrypt_t( args..., EVP_aes_128_ecb() ) {}
    };
    
    class AES_192_ECB : public decrypt_t { public: template< class... T >
          AES_192_ECB( const T&... args ) : decrypt_t( args..., EVP_aes_192_ecb() ) {}
    };
    
    class AES_256_ECB : public decrypt_t { public: template< class... T >
          AES_256_ECB( const T&... args ) : decrypt_t( args..., EVP_aes_256_ecb() ) {}
    };

    /*─······································································─*/
    
    class TRIPLE_DES_CFB : public decrypt_t { public: template< class... T >
          TRIPLE_DES_CFB ( const T&... args ) : decrypt_t( args..., EVP_des_ede3_cfb() ) {}
    };
    
    class TRIPLE_DES_CBC : public decrypt_t { public: template< class... T >
          TRIPLE_DES_CBC ( const T&... args ) : decrypt_t( args..., EVP_des_ede3_cbc() ) {}
    };
    
    class TRIPLE_DES_ECB : public decrypt_t { public: template< class... T >
          TRIPLE_DES_ECB ( const T&... args ) : decrypt_t( args..., EVP_des_ede3_ecb() ) {}
    };

    /*─······································································─*/
    
    class DES_EDE_CFB : public decrypt_t { public: template< class... T >
          DES_EDE_CFB ( const T&... args ) : decrypt_t( args..., EVP_des_ede_cfb() ) {}
    };
    
    class DES_EDE_CBC : public decrypt_t { public: template< class... T >
          DES_EDE_CBC ( const T&... args ) : decrypt_t( args..., EVP_des_ede_cbc() ) {}
    };
    
    class DES_EDE_ECB : public decrypt_t { public: template< class... T >
          DES_EDE_ECB ( const T&... args ) : decrypt_t( args..., EVP_des_ede_ecb() ) {}
    };

    /*─······································································─*/
    
    class DES_CFB : public decrypt_t { public: template< class... T >
          DES_CFB ( const T&... args ) : decrypt_t( args..., EVP_des_cfb() ) {}
    };
    
    class DES_CBC : public decrypt_t { public: template< class... T >
          DES_CBC ( const T&... args ) : decrypt_t( args..., EVP_des_cbc() ) {}
    };
    
    class DES_ECB : public decrypt_t { public: template< class... T >
          DES_ECB ( const T&... args ) : decrypt_t( args..., EVP_des_ecb() ) {}
    };

    /*─······································································─*/
    
    class RC4 : public decrypt_t { public: template< class... T >
          RC4 ( const T&... args ) : decrypt_t( args..., EVP_rc4() ) {}
    };

    /*─······································································─*/

    class RSA : public rsa_t { public: template< class... T > 
          RSA ( const T&... args ) : rsa_t( args... ) {}
    };

}}
    
    /*─······································································─*/

namespace crypto { namespace encode {

    class BASE58 : public encoder_t { public:
          BASE58 () : encoder_t( "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz" ) {}
    };

    class BASE16 : public encoder_t { public: 
          BASE16 () : encoder_t( "123456789ABCDEF" ){}
    };

    class BASE8 : public encoder_t { public: 
          BASE8 () : encoder_t( "1234567" ){}
    };

    class BASE4 : public encoder_t { public: 
          BASE4 () : encoder_t( "123" ){}
    };

    class BASE64 : public enc_base64_t { public:
          BASE64 () : enc_base64_t() {}
    };

}}
    
    /*─······································································─*/

namespace crypto { namespace decode {

    class BASE58 : public decoder_t { public:
          BASE58 () : decoder_t( "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz" ) {}
    };

    class BASE16 : public decoder_t { public: 
          BASE16 () : decoder_t( "123456789ABCDEF" ){}
    };

    class BASE8 : public decoder_t { public: 
          BASE8 () : decoder_t( "1234567" ){}
    };

    class BASE4 : public decoder_t { public: 
          BASE4 () : decoder_t( "123" ){}
    };

    class BASE64 : public dec_base64_t { public:
          BASE64 () : dec_base64_t() {}
    };

}}
    
    /*─······································································─*/

namespace crypto { namespace curve { //openssl ecparam -list_curves
    
    class PRIME256V1: public ec_t { public: template< class... T >
          PRIME256V1( const T&... args ) noexcept : ec_t( args..., NID_X9_62_prime256v1 ) {}
    };
    
    class PRIME192V1 : public ec_t { public: template< class... T >
          PRIME192V1( const T&... args ) noexcept : ec_t( args..., NID_X9_62_prime192v1 ) {}
    };

    /*─······································································─*/
    
    class SECP128R1 : public ec_t { public: template< class... T >
          SECP128R1( const T&... args ) noexcept : ec_t( args..., NID_secp128r1 ) {}
    };
    
    class SECP128R2 : public ec_t { public: template< class... T >
          SECP128R2( const T&... args ) noexcept : ec_t( args..., NID_secp128r2 ) {}
    };

    /*─······································································─*/
    
    class SECP160R1 : public ec_t { public: template< class... T >
          SECP160R1( const T&... args ) noexcept : ec_t( args..., NID_secp160r1 ) {}
    };
    
    class SECP160R2 : public ec_t { public: template< class... T >
          SECP160R2( const T&... args ) noexcept : ec_t( args..., NID_secp160r2 ) {}
    };
    
    class SECP160K1 : public ec_t { public: template< class... T >
          SECP160K1( const T&... args ) noexcept : ec_t( args..., NID_secp160k1 ) {}
    };

    /*─······································································─*/
    
    class SECP256K1 : public ec_t { public: template< class... T >
          SECP256K1( const T&... args ) noexcept : ec_t( args..., NID_secp256k1 ) {}
    };

}}
    
    /*─······································································─*/

namespace crypto { namespace sign {
    
    class DSA : public dsa_t { public: template< class... T >
          DSA ( const T&... args ) : dsa_t ( args... ) {}
    };
    
    class DH : public dh_t { public: template< class... T >
          DH ( const T&... args ) : dh_t ( args... ) {}
    };

}}

}
#endif
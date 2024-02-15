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
#include <openssl/ecdh.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace crypto {

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

public:

    template< class T >
    hash_t( const T& type, ulong length ) : obj( new NODE() ) {
        obj->buff  = ptr_t<uchar>( length );
        obj->ctx   = EVP_MD_CTX_new();
        obj->state = 1;
        if ( !obj->ctx || !EVP_DigestInit_ex( obj->ctx, type, NULL ) )
           { process::error("cant initializate hash_t"); }
    }

    void update( const string_t& msg ) const noexcept { 
        if( obj->state != 1 ){ return; }
        EVP_DigestUpdate( obj->ctx, (uchar*) msg.data(), msg.size() );
    }

    string_t get() const noexcept { 
        force_close(); return { (char*) &obj->buff, obj->length }; 
    }

    string_t get_hex() const noexcept { 
        return crypto::buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_DigestFinal_ex( obj->ctx, &obj->buff, &obj->length );
        EVP_MD_CTX_free( obj->ctx ); //EVP_cleanup();
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 

    virtual ~hash_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }
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

public:

    template< class T >
    hmac_t( const string_t& key, const T& type, ulong length ) : obj( new NODE() ) { 
        obj->buff  = ptr_t<uchar>( length ); 
        obj->ctx   = HMAC_CTX_new(); 
        obj->state = 1;
        if ( !obj->ctx || !HMAC_Init_ex( obj->ctx, key.c_str(), key.size(), type, nullptr ) )
           { process::error("cant initializate hmac_t"); }
    }

    void update( const string_t& msg ) const noexcept { 
        if( obj->state != 1 ){ return; }
        HMAC_Update( obj->ctx, (uchar*) msg.data(), msg.size() ); 
    }

    string_t get() const noexcept { 
        force_close(); return { (char*) &obj->buff, obj->length }; 
    }

    string_t get_hex() const noexcept { 
        return crypto::buff2hex( this->get() );
    }

    void force_close() const noexcept {
        if( obj->state == 0 ){ return; } obj->state = 0;
        HMAC_Final( obj->ctx, &obj->buff, &obj->length ); 
        HMAC_CTX_free( obj->ctx ); //EVP_cleanup();
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 
    
    virtual ~hmac_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }
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

public:

    event_t<string_t> onData;
    event_t<>         onClose;

    template< class T >
    encrypt_t( const string_t& iv, const string_t& key, const T& type ) : obj( new NODE() ) {
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx   = EVP_CIPHER_CTX_new(); 
        obj->state = 1; 
        if ( !obj->ctx || !EVP_EncryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), (uchar*)iv.data() ) )
           { process::error("cant initializate encrypt_t"); }
    }

    template< class T >
    encrypt_t( const string_t& key, const T& type ) : obj( new NODE() ) {
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx   = EVP_CIPHER_CTX_new(); 
        obj->state = 1; 
        if ( !obj->ctx || !EVP_EncryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), NULL ) )
           { process::error("cant initializate encrypt_t"); }
    }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_EncryptUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size() );
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); }}
    }

    string_t get() const noexcept { 
        force_close(); return obj->buff; 
    }

    string_t get_hex() const noexcept { 
        return crypto::buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_EncryptFinal( obj->ctx, &obj->bff, &obj->len );
        EVP_CIPHER_CTX_free( obj->ctx ); //EVP_cleanup();
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); } onClose.emit(); }
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 
    
    virtual ~encrypt_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

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
    decrypt_t( const string_t& iv, const string_t& key, const T& type ) : obj( new NODE() ) {
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx   = EVP_CIPHER_CTX_new(); 
        obj->state = 1;
        if ( !obj->ctx || !EVP_DecryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), (uchar*)iv.data() ) )
           { process::error("cant initializate decrypt_t"); }
    }

    template< class T >
    decrypt_t( const string_t& key, const T& type ) : obj( new NODE() ) {
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx   = EVP_CIPHER_CTX_new(); 
        obj->state = 1;
        if ( !obj->ctx || !EVP_DecryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), NULL ) )
           { process::error("cant initializate decrypt_t"); }
    }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_DecryptUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size());
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); }}
    }

    string_t get() const noexcept { 
        force_close(); return obj->buff; 
    }

    string_t get_hex() const noexcept {  
        return crypto::buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_DecryptFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_CIPHER_CTX_free( obj->ctx ); //EVP_cleanup();
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); } onClose.emit(); }
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 
    
    virtual ~decrypt_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

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

    enc_base64_t() : obj( new NODE() ) {
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,0);
        obj->ctx   = EVP_ENCODE_CTX_new();
        obj->state = 1;
        if ( !obj->ctx )
           { process::error("cant initializate base64 encoder"); }
        EVP_EncodeInit( obj->ctx );
    }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_EncodeUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size()); 
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); }}
    }

    string_t get() const noexcept { 
        force_close(); return obj->buff; 
    }

    string_t get_hex() const noexcept {  
        return crypto::buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_EncodeFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_ENCODE_CTX_free( obj->ctx ); //EVP_cleanup();
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); } onClose.emit(); }
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 
    
    virtual ~enc_base64_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

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

    encoder_t( const string_t& chr ) : obj( new NODE() ) {
        obj->state = 1; obj->chr = chr; 
        obj->bn = (BIGNUM*) BN_new();
        if ( !obj->bn )
           { process::error("cant initializate encoder"); }
    }

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

    string_t get() const noexcept { 
        force_close(); return obj->buff; 
    }

    string_t get_hex() const noexcept {  
        return crypto::buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; }
            obj->state  = 0;
        BN_clear_free( obj->bn );
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); }  
    
    virtual ~encoder_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

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

    dec_base64_t() : obj( new NODE() ) {
        obj->bff   = ptr_t<uchar>(UNBFF_SIZE,0);
        obj->ctx   = EVP_ENCODE_CTX_new();
        obj->state = 1; 
        if ( !obj->ctx )
           { process::error("cant initializate base64 decoder"); }
        EVP_DecodeInit( obj->ctx );
    }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_DecodeUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size()); 
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); }}
    }

    string_t get() const noexcept { 
        force_close(); return obj->buff; 
    }

    string_t get_hex() const noexcept {  
        return crypto::buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_DecodeFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_ENCODE_CTX_free( obj->ctx ); //EVP_cleanup();
        if ( obj->len > 0 ) { if ( onData.empty() ) {
                 obj->buff += string_t( (char*)&obj->bff, (ulong) obj->len );
        } else { onData.emit( string_t( (char*)&obj->bff, (ulong) obj->len ) ); } onClose.emit(); }
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 
    
    virtual ~dec_base64_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

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

    decoder_t( const string_t& chr ) : obj( new NODE() ) {
        obj->state = 1; obj->chr = chr; 
        obj->bn = (BIGNUM*) BN_new();
        if ( !obj->bn )
           { process::error("cant initializate decoder"); }
    }

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

    string_t get() const noexcept { 
        force_close(); return obj->buff; 
    }

    string_t get_hex() const noexcept { 
        return crypto::buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 1 ){ return; } obj->state = 0;
         BN_clear_free( obj->bn ); onClose.emit();
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 
    
    virtual ~decoder_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

class ecdh_t {
protected:

    struct NODE {
        EC_POINT *pub_key  = nullptr;
        BIGNUM   *priv_key = nullptr;
        EC_KEY   *key_pair = nullptr;
        int       state;
    };  ptr_t<NODE> obj;
    
public:

    template< class T >
    ecdh_t( const string_t& key, const T& type ) noexcept : obj( new NODE() ) {
        obj->state = 1;

        obj->key_pair = EC_KEY_new_by_curve_name(type);
                        EC_KEY_generate_key( obj->key_pair );

        obj->priv_key = (BIGNUM*) BN_new(); 
        BN_hex2bn( &obj->priv_key, key.c_str() );
        EC_KEY_set_private_key( obj->key_pair, obj->priv_key );

        const EC_POINT* pub_key = EC_KEY_get0_public_key( obj->key_pair );
        obj->pub_key = EC_POINT_dup(pub_key, EC_KEY_get0_group(obj->key_pair));
    }

    template< class T >
    ecdh_t( const T& type ) noexcept : obj( new NODE() ) {
        obj->state = 1;

        obj->key_pair = EC_KEY_new_by_curve_name(type);
                        EC_KEY_generate_key( obj->key_pair );

        obj->pub_key  = EC_KEY_get0_public_key( obj->key_pair );
        obj->priv_key = EC_KEY_get0_private_key( obj->key_pair );
    }

    string_t get_public_key() const noexcept { 
        if( obj->state != 1 ){ return ""; } uchar *key = NULL;
        int len = i2o_ECPublicKey( obj->key_pair , &key );
        return { (char*) &key, (ulong) len };
    }

    string_t get_public_key_hex() const noexcept {
        if( obj->state != 1 ){ return ""; }
        return crypto::buff2hex( this->get_public_key() );
    }

    string_t get_private_key() const noexcept { 
        if( obj->state != 1 ){ return ""; } uchar *key = NULL;
        int len = i2d_ECPrivateKey( obj->key_pair , &key ); 
        return { (char*) &key, (ulong) len };
    }

    string_t get_private_key_hex() const noexcept {
        if( obj->state != 1 ){ return ""; }
        return crypto::buff2hex( this->get_private_key() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        if( obj->priv_key != nullptr ) BN_free( obj->priv_key );
    //  if( obj->key_pair != nullptr ) EC_KEY_free( obj->key_pair );
        if( obj->pub_key  != nullptr ) EC_POINT_free( obj->pub_key );
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 
    
    virtual ~ecdh_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

class ecdsa_t {
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
    ecdsa_t( const string_t& key, const T& type ) noexcept : obj( new NODE() ) {
        obj->state = 1;

        obj->key_pair  = EC_KEY_new_by_curve_name(type);
        obj->key_group = EC_GROUP_new_by_curve_name(type);

        obj->priv_key = (BIGNUM*) BN_new(); 
        BN_hex2bn( &obj->priv_key, key.c_str() );
        EC_KEY_set_private_key( obj->key_pair, obj->priv_key );

        obj->pub_key = (EC_POINT*) EC_POINT_new( obj->key_group );
        EC_POINT_mul( obj->key_group, obj->pub_key, obj->priv_key, nullptr, nullptr, nullptr );
        EC_KEY_set_public_key( obj->key_pair, obj->pub_key );

    }

    template< class T >
    ecdsa_t( const T& type ) noexcept : obj( new NODE() ) {
        obj->state = 1;

        obj->key_pair  = EC_KEY_new();
        obj->key_group = EC_GROUP_new_by_curve_name( type );

        EC_KEY_set_group( obj->key_pair, obj->key_group );
        EC_KEY_generate_key( obj->key_pair );

        obj->pub_key  = (EC_POINT*) EC_KEY_get0_public_key( obj->key_pair );
        obj->priv_key = (BIGNUM*)  EC_KEY_get0_private_key( obj->key_pair );
    }

    string_t get_public_key( uint x = 0 ) const noexcept {
        if( obj->state != 1 ){ return ""; }
        return crypto::hex2buff( get_public_key_hex(x) );
    }

    string_t get_private_key() const noexcept {
        if( obj->state != 1 ){ return ""; }
        return crypto::hex2buff( get_private_key_hex() );
    }

    string_t get_public_key_hex( uint x = 0 ) const noexcept { 
        if( obj->state != 1 ){ return ""; }
        point_conversion_form_t y; switch( x ){
            case 0:  y = POINT_CONVERSION_HYBRID;       break;
            case 1:  y = POINT_CONVERSION_COMPRESSED;   break;
            default: y = POINT_CONVERSION_UNCOMPRESSED; break;
        }   return EC_POINT_point2hex( obj->key_group, obj->pub_key, y, nullptr );
    }

    string_t get_private_key_hex() const noexcept { return BN_bn2hex( obj->priv_key ); }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        if( obj->priv_key  != nullptr ) BN_free( obj->priv_key );
    //  if( obj->key_pair  != nullptr ) EC_KEY_free( obj->key_pair );
        if( obj->pub_key   != nullptr ) EC_POINT_free( obj->pub_key );
        if( obj->key_group != nullptr ) EC_GROUP_free( obj->key_group );
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 
    
    virtual ~ecdsa_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

class rsa_t {
protected:

    struct NODE {
        RSA*    rsa = nullptr;
        BIGNUM* num = nullptr;
        int   state;
    };  ptr_t<NODE> obj;
    
public:

    template< class T >
    rsa_t() : obj( new NODE() ) {
        obj->rsa   = RSA_new();
        obj->num   =  BN_new();
        obj->state = 1;
        if ( !obj->num || !obj->rsa )
           { process::error("creating rsa object"); }
    }

    int generate_key( int keyLen ) const noexcept {
                BN_set_word( obj->num, RSA_F4 );
        return RSA_generate_key_ex( obj->rsa, keyLen, obj->num, NULL );
    }

    int write_private_key( const string_t& path ) const noexcept {
        FILE* fp = fopen( path.c_str() , "wb"); int res = 0;
        res = PEM_write_RSAPrivateKey( fp, obj->rsa, NULL, NULL, 0, NULL, NULL);
        fclose( fp ); return res;
    }

    int write_public_key( const string_t& path ) const noexcept {
        FILE* fp = fopen( path.c_str() , "wb"); int res = 0;
        res = PEM_write_RSAPublicKey( fp, obj->rsa );
        fclose( fp ); return res;
    }

    void set_private_key( const string_t& path ) const noexcept {
        FILE* fp = fopen( path.c_str(), "r" );
        PEM_read_RSAPrivateKey( fp, &obj->rsa, NULL, NULL ); fclose( fp );
    }

    void set_public_key( const string_t& path ) const noexcept {
        FILE* fp = fopen( path.c_str(), "r" );
        PEM_read_RSA_PUBKEY( fp, &obj->rsa, NULL, NULL ); fclose( fp );
    }

    string_t encrypt( const string_t& msg, int padding=0 ) const noexcept {
        if( msg.empty() || padding < 0 || obj->state == 0 ){ return ""; }
        int          len = RSA_size( obj->rsa );
        ptr_t<uchar> out = new uchar[len];
        ulong y = 0; int c = 0;
        string_t     bff; 
        do { c=RSA_private_decrypt( msg.size()+y, (uchar*)msg.c_str()-y, &out, obj->rsa, padding );
             if( c>0 ){ y+=c; } process::next(); 
        } while( (ulong)c < msg.size() ); return bff;
    }

    string_t decrypt( const string_t& msg, int padding=0 ) const noexcept {
        if( msg.empty() || padding < 0 || obj->state == 0 ){ return ""; }
        int          len = RSA_size( obj->rsa );
        ptr_t<uchar> out = new uchar[len];
        ulong y = 0; int c = 0;
        string_t     bff; 
        do { c=RSA_public_encrypt( msg.size()+y, (uchar*)msg.c_str()-y, &out, obj->rsa, padding );
             if( c>0 ){ y+=c; } process::next(); 
        } while( (ulong)c < msg.size() ); return bff;
        return bff;
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        if( obj->rsa != nullptr ) RSA_free( obj->rsa );
        if( obj->num != nullptr )  BN_free( obj->num );
    }
    
    virtual ~rsa_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

class dh_t {
protected:

    struct NODE {
        DH* dh;
        BIGNUM* g;
        BIGNUM* k;
        int state;
    };  ptr_t<NODE> obj;

public:

    dh_t() { obj->state = 1;
        obj->dh    = DH_new(); 
        obj->g     = BN_new();
        obj->k     = BN_new();
        obj->state = 1;
        if( !obj->dh || !obj->g )
          { process::error( "creating new dh_t" ); }
        if( !DH_check( obj->dh, nullptr ) )
          { process::error( "while checking dh" ); }
        if( !DH_generate_key( obj->dh ) )
          { process::error( "while generating dh params" ); }
    }

    int set_public_key( const string_t& key ) const noexcept {
        if( obj->state != 1 ){ return 0; }
               BN_hex2bn( &obj->k, key.c_str() );
        return DH_set0_key( obj->dh, nullptr, obj->k );
    }

    int set_private_key( const string_t& key ) const noexcept {
        if( obj->state != 1 ){ return 0; }
               BN_hex2bn( &obj->k, key.c_str() );
        return DH_set0_key( obj->dh, obj->k, nullptr );
    }

    string_t get_private_key() const noexcept {
        if( obj->state != 1 ){ return ""; } 
        return BN_bn2hex( DH_get0_priv_key( obj->dh ) );
    }

    string_t get_public_key() const noexcept {
        if( obj->state != 1 ){ return ""; } 
        return BN_bn2hex( DH_get0_pub_key( obj->dh ) );
    }

    int check() const noexcept {
        if( obj->state != 1 ){ return 0; } 
        return DH_check( obj->dh, NULL );
    }

    string_t compute_key( const string_t& key ) const noexcept {
        if( obj->state != 1 ){ return ""; } 
        ptr_t<uchar> shared ( DH_size( obj->dh ) );
                  BN_hex2bn( &obj->k, key.c_str() );
        int len = DH_compute_key( &shared, obj->k, obj->dh );
        return string_t( (char*) &shared, (ulong) len );
    }

    void force_close() const noexcept {
        if( obj->state == 0 ){ return; } obj->state = 0;
        DH_free( obj->dh ); BN_free( obj->k ); BN_free( obj->g );
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

class dsa_t {
protected:

    struct NODE {
        DSA    *dsa = nullptr;
        uint    len = 512;
        int     state;
    };  ptr_t<NODE> obj;
    
public:

    template< class T >
    dsa_t( uint size ) : obj( new NODE() ) {
        obj->state = 1; obj->len = size; obj->dsa = DSA_new(); 
        if(!DSA_generate_parameters_ex( obj->dsa, obj->len, NULL, 0, NULL, NULL, NULL ) )
          { process::error("while generating DSA parameters"); }
        if(!DSA_generate_key( obj->dsa ) )
          { process::error("while generating DSA key"); }

    }

    template< class T >
    dsa_t( const string_t& path, uint size ) : obj( new NODE() ) { obj->state = 1;
        obj->len = size; obj->dsa = DSA_new(); FILE* fp = fopen(path.c_str(),"r");
        if ( fp == nullptr ) process::error("such file or directory does not exist");
        obj->dsa = PEM_read_DSAPrivateKey( fp, &obj->dsa, nullptr, nullptr );
        fclose( fp ); if( obj->dsa==nullptr ) process::error("while creating DSA");
    }

    string_t sign( const string_t& msg ) const noexcept {
        if( obj->state != 1 ){ return ""; } ptr_t<uchar> sgn( DSA_size(obj->dsa) ); uint len;
        DSA_sign( 0, (uchar*)msg.c_str(), msg.size(), &sgn, &len, obj->dsa );
        return { (char*) &sgn, (ulong) len };
    }

    int verify( const string_t& msg, const string_t& sgn ) const noexcept {
        return DSA_verify( 0, (uchar*)msg.data(), msg.size(), (uchar*)sgn.data(), sgn.size(), obj->dsa );
    }

    void save_private_key( const string_t& path ) const {
        if( obj->state != 1 ){ return; } FILE* fp = fopen( path.c_str(), "w" );
        if ( fp == nullptr ) { process::error("while creating file"); }
        if (!PEM_write_DSA_PUBKEY( fp, obj->dsa ) ) 
           { fclose( fp ); process::error("while writting the private key"); } fclose( fp );
    }

    void save_public_key( const string_t& path ) const {
        if( obj->state != 1 ){ return; } FILE* fp = fopen( path.c_str(), "w" );
        if ( fp == nullptr ) { process::error("while creating file"); }
        if (!PEM_write_DSAPrivateKey( fp, obj->dsa, nullptr, nullptr, 0, nullptr, nullptr ) )
           { fclose( fp ); process::error("while writting the public key"); } fclose( fp );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        if( obj->dsa != nullptr ) DSA_free( obj->dsa );
    }

    bool is_available() const noexcept { return obj->state == 1; }

    bool is_closed() const noexcept { return obj->state == 0; }

    void close() const noexcept { force_close(); } 
    
    virtual ~dsa_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

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

    class RIPEMD160 : public hmac_t { public:
          RIPEMD160( const string_t& key ) : hmac_t( key, EVP_ripemd160(), RIPEMD160_DIGEST_LENGTH ) {}
    }; 

}}
    
    /*─······································································─*/

namespace crypto { namespace enc {
    
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

namespace crypto { namespace dec {
    
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

namespace crypto { namespace enc {

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

namespace crypto { namespace dec {

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

namespace crypto { namespace ecdh { //openssl ecparam -list_curves
    
    class SECP128R1 : public ecdh_t { public: template< class... T >
          SECP128R1( const T&... args ) noexcept : ecdh_t( args..., NID_secp128r1 ) {}
    };
    
    class SECP128R2 : public ecdh_t { public: template< class... T >
          SECP128R2( const T&... args ) noexcept : ecdh_t( args..., NID_secp128r2 ) {}
    };

    /*─······································································─*/
    
    class SECP160R1 : public ecdh_t { public: template< class... T >
          SECP160R1( const T&... args ) noexcept : ecdh_t( args..., NID_secp160r1 ) {}
    };
    
    class SECP160R2 : public ecdh_t { public: template< class... T >
          SECP160R2( const T&... args ) noexcept : ecdh_t( args..., NID_secp160r2 ) {}
    };
    
    class SECP160K1 : public ecdh_t { public: template< class... T >
          SECP160K1( const T&... args ) noexcept : ecdh_t( args..., NID_secp160k1 ) {}
    };

    /*─······································································─*/
    
    class SECP256K1 : public ecdh_t { public: template< class... T >
          SECP256K1( const T&... args ) noexcept : ecdh_t( args..., NID_secp256k1 ) {}
    };

}}
    
    /*─······································································─*/

namespace crypto { namespace ecdsa { //openssl ecparam -list_curves
    
    class SECP128R1 : public ecdsa_t { public: template< class... T >
          SECP128R1( const T&... args ) noexcept : ecdsa_t( args..., NID_secp128r1 ) {}
    };
    
    class SECP128R2 : public ecdsa_t { public: template< class... T >
          SECP128R2( const T&... args ) noexcept : ecdsa_t( args..., NID_secp128r2 ) {}
    };

    /*─······································································─*/
    
    class SECP160R1 : public ecdsa_t { public: template< class... T >
          SECP160R1( const T&... args ) noexcept : ecdsa_t( args..., NID_secp160r1 ) {}
    };
    
    class SECP160R2 : public ecdsa_t { public: template< class... T >
          SECP160R2( const T&... args ) noexcept : ecdsa_t( args..., NID_secp160r2 ) {}
    };
    
    class SECP160K1 : public ecdsa_t { public: template< class... T >
          SECP160K1( const T&... args ) noexcept : ecdsa_t( args..., NID_secp160k1 ) {}
    };

    /*─······································································─*/
    
    class SECP256K1 : public ecdsa_t { public: template< class... T >
          SECP256K1( const T&... args ) noexcept : ecdsa_t( args..., NID_secp256k1 ) {}
    };

}}
    
    /*─······································································─*/

namespace crypto { namespace DH {
    
    class DH : public dh_t { public: template< class... T >
          DH ( const T&... args ) : dh_t( args... ) {}
    };

}}
    
    /*─······································································─*/

namespace crypto { namespace sign {
    
    class DSA : public ecdsa_t { public: template< class... T >
          DSA ( const T&... args ) : dsa_t( args... ) {}
    };

}}

}
#endif
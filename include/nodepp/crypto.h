#ifndef NODEPP_CRYPTO
#define NODEPP_CRYPTO

#ifndef OPENSSL_API_COMPAT
#define OPENSSL_API_COMPAT 0x00908000L
#endif

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

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto {

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
}

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class hash_t {
protected:

    struct _str_ {
        ptr_t<uchar> buffer;
        EVP_MD_CTX* ctx;
        uint length;
        int  state;
    };  ptr_t<_str_> obj = new _str_();

public:

    template< class T >
    hash_t( const T& type, ulong length ) noexcept : obj( new _str_() ) {
        obj->buffer = ptr_t<uchar>( length );
        obj->ctx    = EVP_MD_CTX_new();
        obj->state  = 1;
        EVP_DigestInit_ex( obj->ctx, type, NULL );
    }

    void update( const string_t& msg ) const noexcept { 
        if( obj->state != 1 ){ return; }
        EVP_DigestUpdate( obj->ctx, (uchar*) msg.data(), msg.size() );
    }

    string_t get() const noexcept { 
        force_close(); return { (char*) &obj->buffer, obj->length }; 
    }

    string_t get_hex() const noexcept { 
        return buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_DigestFinal_ex( obj->ctx, &obj->buffer, &obj->length );
        EVP_MD_CTX_free( obj->ctx ); //EVP_cleanup();
    }

    void free() const noexcept { force_close(); } 

    virtual ~hash_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }
};  }

namespace crypto { namespace hash {

    class MD4 : public hash_t { public:
          MD4() noexcept : hash_t( EVP_md4(), MD4_DIGEST_LENGTH ) {}
    };

    class MD5 : public hash_t { public:
          MD5() noexcept : hash_t( EVP_md5(), MD5_DIGEST_LENGTH ) {}
    };

    class SHA1 : public hash_t { public:
          SHA1() noexcept : hash_t( EVP_sha1(), SHA_DIGEST_LENGTH ) {}
    };

    class SHA256 : public hash_t { public:
          SHA256() noexcept : hash_t( EVP_sha256(), SHA256_DIGEST_LENGTH ) {}
    };

    class SHA384 : public hash_t { public:
          SHA384() noexcept : hash_t( EVP_sha384(), SHA384_DIGEST_LENGTH ) {}
    };

    class SHA512 : public hash_t { public:
          SHA512() noexcept : hash_t( EVP_sha512(), SHA512_DIGEST_LENGTH ) {}
    };

    class RIPEMD160 : public hash_t { public:
          RIPEMD160() noexcept : hash_t( EVP_ripemd160(), RIPEMD160_DIGEST_LENGTH ) {}
    }; 

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class hmac_t {
protected:

    struct _str_ {
        ptr_t<uchar> buffer;
        HMAC_CTX* ctx; 
        uint length;
        int  state;
    };  ptr_t<_str_> obj;

public:

    template< class T >
    hmac_t( const string_t& key, const T& type, ulong length ) noexcept : obj( new _str_() ) { 
        obj->buffer = ptr_t<uchar>( length ); obj->ctx = HMAC_CTX_new(); obj->state = 1;
        HMAC_Init_ex( obj->ctx, key.c_str(), key.size(), type, nullptr );
    }

    void update( const string_t& msg ) const noexcept { 
        if( obj->state != 1 ){ return; }
        HMAC_Update( obj->ctx, (uchar*) msg.data(), msg.size() ); 
    }

    string_t get() const noexcept { 
        force_close(); return { (char*) &obj->buffer, obj->length }; 
    }

    string_t get_hex() const noexcept { 
        return buff2hex( this->get() );
    }

    void force_close() const noexcept {
        if( obj->state == 0 ){ return; } obj->state = 0;
        HMAC_Final( obj->ctx, &obj->buffer, &obj->length ); 
        HMAC_CTX_free( obj->ctx ); //EVP_cleanup();
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~hmac_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }
};  }

namespace crypto { namespace hmac {

    class MD4 : public hmac_t { public:
          MD4 ( const string_t& key ) noexcept : hmac_t( key, EVP_md4(), MD4_DIGEST_LENGTH ) {}
    };

    class MD5 : public hmac_t { public:
          MD5 ( const string_t& key ) noexcept : hmac_t( key, EVP_md5(), MD5_DIGEST_LENGTH ) {}
    };

    class SHA1 : public hmac_t { public:
          SHA1 ( const string_t& key ) noexcept : hmac_t( key, EVP_sha1(), SHA_DIGEST_LENGTH ) {}
    };

    class SHA256 : public hmac_t { public:
          SHA256 ( const string_t& key ) noexcept : hmac_t( key, EVP_sha256(), SHA256_DIGEST_LENGTH ) {}
    };

    class SHA384 : public hmac_t { public:
          SHA384 ( const string_t& key ) noexcept : hmac_t( key, EVP_sha384(), SHA384_DIGEST_LENGTH ) {}
    };

    class SHA512 : public hmac_t { public:
          SHA512 ( const string_t& key ) noexcept : hmac_t( key, EVP_sha512(), SHA512_DIGEST_LENGTH ) {}
    };

    class RIPEMD160 : public hmac_t { public:
          RIPEMD160( const string_t& key ) noexcept : hmac_t( key, EVP_ripemd160(), RIPEMD160_DIGEST_LENGTH ) {}
    }; 

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class encrypt_t {
protected:

    struct _str_ {
        EVP_CIPHER_CTX* ctx;
        ptr_t<uchar> bff;
        string_t buffer;
        int state, len;
    };  ptr_t<_str_> obj;

public:

    template< class T >
    encrypt_t( const string_t& iv, const string_t& key, const T& type ) noexcept : obj( new _str_() ) {
        obj->bff    = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx    = EVP_CIPHER_CTX_new(); 
        obj->state  = 1; 
        EVP_EncryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), (uchar*)iv.data() );
    }

    template< class T >
    encrypt_t( const string_t& key, const T& type ) noexcept : obj( new _str_() ) {
        obj->bff    = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx    = EVP_CIPHER_CTX_new(); 
        obj->state  = 1; 
        EVP_EncryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), NULL );
    }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_EncryptUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size() );
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong)obj->len };
    }

    string_t get() const noexcept { 
        force_close(); return obj->buffer; 
    }

    string_t get_hex() const noexcept {  
        return buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_EncryptFinal( obj->ctx, &obj->bff, &obj->len );
        EVP_CIPHER_CTX_free( obj->ctx ); //EVP_cleanup();
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~encrypt_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};  }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace enc {
    
    class AES_128_CBC : public encrypt_t { public: template< class... T >
          AES_128_CBC( const T&... args ) noexcept : encrypt_t( args..., EVP_aes_128_cbc() ) {}
    };
    
    class AES_192_CBC : public encrypt_t { public: template< class... T >
          AES_192_CBC( const T&... args ) noexcept : encrypt_t( args..., EVP_aes_192_cbc() ) {}
    };
    
    class AES_256_CBC : public encrypt_t { public: template< class... T >
          AES_256_CBC( const T&... args ) noexcept : encrypt_t( args..., EVP_aes_256_cbc() ) {}
    };

    /*─······································································─*/
    
    class AES_128_ECB : public encrypt_t { public: template< class... T >
          AES_128_ECB( const T&... args ) noexcept : encrypt_t( args..., EVP_aes_128_ecb() ) {}
    };
    
    class AES_192_ECB : public encrypt_t { public: template< class... T >
          AES_192_ECB( const T&... args ) noexcept : encrypt_t( args..., EVP_aes_192_ecb() ) {}
    };
    
    class AES_256_ECB : public encrypt_t { public: template< class... T >
          AES_256_ECB( const T&... args ) noexcept : encrypt_t( args..., EVP_aes_256_ecb() ) {}
    };

    /*─······································································─*/
    
    class DES_CBC : public encrypt_t { public: template< class... T >
          DES_CBC ( const T&... args ) noexcept : encrypt_t( args..., EVP_des_cbc() ) {}
    };
    
    class DES_ECB : public encrypt_t { public: template< class... T >
          DES_ECB ( const T&... args ) noexcept : encrypt_t( args..., EVP_des_ecb() ) {}
    };

    /*─······································································─*/
    
    class RC4 : public encrypt_t { public: template< class... T >
          RC4 ( const T&... args ) noexcept : encrypt_t( args..., EVP_rc4() ) {}
    };

}   }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class decrypt_t {
protected:

    struct _str_ {
        EVP_CIPHER_CTX* ctx; 
        ptr_t<uchar> bff;
        string_t buffer;
        int state, len;
    };  ptr_t<_str_> obj;
    
public:

    template< class T >
    decrypt_t( const string_t& iv, const string_t& key, const T& type ) noexcept : obj( new _str_() ) {
        obj->bff    = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx    = EVP_CIPHER_CTX_new(); 
        obj->state  = 1;
        EVP_DecryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), (uchar*)iv.data() );
    }

    template< class T >
    decrypt_t( const string_t& key, const T& type ) noexcept : obj( new _str_() ) {
        obj->bff    = ptr_t<uchar>(UNBFF_SIZE,'\0');
        obj->ctx    = EVP_CIPHER_CTX_new(); 
        obj->state  = 1;
        EVP_DecryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), NULL );
    }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_DecryptUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size());
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    string_t get() const noexcept { 
        force_close(); return obj->buffer; 
    }

    string_t get_hex() const noexcept {  
        return buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_DecryptFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_CIPHER_CTX_free( obj->ctx ); //EVP_cleanup();
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~decrypt_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};  }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace dec {
    
    class AES_128_CBC : public decrypt_t { public: template< class... T >
          AES_128_CBC( const T&... args ) noexcept : decrypt_t( args..., EVP_aes_128_cbc() ) {}
    };
    
    class AES_192_CBC : public decrypt_t { public: template< class... T >
          AES_192_CBC( const T&... args ) noexcept : decrypt_t( args..., EVP_aes_192_cbc() ) {}
    };
    
    class AES_256_CBC : public decrypt_t { public: template< class... T >
          AES_256_CBC( const T&... args ) noexcept : decrypt_t( args..., EVP_aes_256_cbc() ) {}
    };

    /*─······································································─*/
    
    class AES_128_ECB : public decrypt_t { public: template< class... T >
          AES_128_ECB( const T&... args ) noexcept : decrypt_t( args..., EVP_aes_128_ecb() ) {}
    };
    
    class AES_192_ECB : public decrypt_t { public: template< class... T >
          AES_192_ECB( const T&... args ) noexcept : decrypt_t( args..., EVP_aes_192_ecb() ) {}
    };
    
    class AES_256_ECB : public decrypt_t { public: template< class... T >
          AES_256_ECB( const T&... args ) noexcept : decrypt_t( args..., EVP_aes_256_ecb() ) {}
    };

    /*─······································································─*/
    
    class DES_CBC : public decrypt_t { public: template< class... T >
          DES_CBC ( const T&... args ) noexcept : decrypt_t( args..., EVP_des_cbc() ) {}
    };
    
    class DES_ECB : public decrypt_t { public: template< class... T >
          DES_ECB ( const T&... args ) noexcept : decrypt_t( args..., EVP_des_ecb() ) {}
    };

    /*─······································································─*/
    
    class RC4 : public decrypt_t { public: template< class... T >
          RC4 ( const T&... args ) noexcept : decrypt_t( args..., EVP_rc4() ) {}
    };

}   }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class enc_base64_t {
protected:

    struct _str_ {
        EVP_ENCODE_CTX* ctx; 
        ptr_t<uchar> bff;
        string_t buffer;
        int state, len;
    };  ptr_t<_str_> obj;
    
public:

    enc_base64_t() noexcept : obj( new _str_() ) {
        obj->bff    = ptr_t<uchar>(UNBFF_SIZE,0);
        obj->ctx    = EVP_ENCODE_CTX_new();
        obj->state  = 1;
        EVP_EncodeInit( obj->ctx );
    }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_EncodeUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size()); 
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    string_t get() const noexcept { 
        force_close(); return obj->buffer; 
    }

    string_t get_hex() const noexcept {  
        return buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_EncodeFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_ENCODE_CTX_free( obj->ctx ); //EVP_cleanup();
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~enc_base64_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};  }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class encoder_t {
protected:

    struct _str_ {
        BIGNUM* bn;
        string_t chr;
        string_t buff;
    };  ptr_t<_str_> obj;

public:

    encoder_t( string_t chr ) noexcept : obj( new _str_() ) {
        obj->chr = chr; obj->bn = (BIGNUM*) BN_new();
    }

    void update( const string_t& msg ) const noexcept { 
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
        return obj->buff; 
    }

    string_t get_hex() const noexcept {  
        return buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        BN_clear_free( obj->bn );
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~encoder_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace enc { 

    class BASE58 : public encoder_t { public:
          BASE58 () noexcept : encoder_t( "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz" ) {}
    };

    class BASE16 : public encoder_t { public: 
          BASE16 () noexcept : encoder_t( "123456789ABCDEF" ){}
    };

    class BASE8 : public encoder_t { public: 
          BASE8 () noexcept : encoder_t( "1234567" ){}
    };

    class BASE4 : public encoder_t { public: 
          BASE4 () noexcept : encoder_t( "123" ){}
    };

    class BASE2 : public encoder_t { public: 
          BASE2 () noexcept : encoder_t( "1" ){}
    };

    class BASE64 : public enc_base64_t { public:
          BASE64 () noexcept : enc_base64_t() {}
    };

}   }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class dec_base64_t {
protected:

    struct _str_ {
        EVP_ENCODE_CTX* ctx; 
        ptr_t<uchar> bff;
        string_t buffer;
        int state, len;
    };  ptr_t<_str_> obj;

public:

    dec_base64_t() noexcept : obj( new _str_() ) {
        obj->bff    = ptr_t<uchar>(UNBFF_SIZE,0);
        obj->ctx    = EVP_ENCODE_CTX_new();
        obj->state  = 1;
        EVP_DecodeInit( obj->ctx );
    }

    void update( const string_t& msg ) const noexcept { if( obj->state != 1 ){ return; }
        EVP_DecodeUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size()); 
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    string_t get() const noexcept { 
        force_close(); return obj->buffer; 
    }

    string_t get_hex() const noexcept {  
        return buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_DecodeFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_ENCODE_CTX_free( obj->ctx ); //EVP_cleanup();
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~dec_base64_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};  }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class decoder_t {
protected:

    struct _str_ {
        BIGNUM* bn;
        string_t chr;
        string_t buff;
    };  ptr_t<_str_> obj;

public:

    decoder_t( string_t chr ) noexcept : obj( new _str_() ) {
        obj->chr = chr; obj->bn = (BIGNUM*) BN_new();
    }

    void update( const string_t& msg ) const { 

        for( const auto& c : msg ) {
            const char* pos = strchr( obj->chr.data(), c );
            if( pos == nullptr ) _Error("Invalid BaseX character");

            BN_mul_word( obj->bn, obj->chr.size() );
            BN_add_word( obj->bn, pos - obj->chr.data() );
        }

        ptr_t<uchar> out ( BN_num_bytes(obj->bn) );
        BN_bn2bin( obj->bn, out.data() );
        obj->buff += (string_t){ (char*) &out, out.size() };
    }

    string_t get() const noexcept { return obj->buff; }

    string_t get_hex() const noexcept {  
        return buff2hex( this->get() );
    }

    void force_close() const noexcept { 
        BN_clear_free( obj->bn );
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~decoder_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace dec { 

    class BASE58 : public decoder_t { public:
          BASE58 () noexcept : decoder_t( "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz" ) {}
    };

    class BASE16 : public decoder_t { public: 
          BASE16 () noexcept : decoder_t( "123456789ABCDEF" ){}
    };

    class BASE8 : public decoder_t { public: 
          BASE8 () noexcept : decoder_t( "1234567" ){}
    };

    class BASE4 : public decoder_t { public: 
          BASE4 () noexcept : decoder_t( "123" ){}
    };

    class BASE2 : public decoder_t { public: 
          BASE2 () noexcept : decoder_t( "1" ){}
    };

    class BASE64 : public dec_base64_t { public:
          BASE64 () noexcept : dec_base64_t() {}
    };

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class ecdh_t {
protected:

    struct _str_ {
        EC_POINT *pub_key   = nullptr;
        BIGNUM   *priv_key  = nullptr;
        EC_KEY   *key_pair  = nullptr;
    };  ptr_t<_str_> obj;
    
public:

    template< class T >
    ecdh_t( const string_t& key, const T& type ) noexcept : obj( new _str_() ) {

        obj->key_pair = EC_KEY_new_by_curve_name(type);
                        EC_KEY_generate_key( obj->key_pair );

        obj->priv_key = (BIGNUM*) BN_new(); 
        BN_hex2bn( &obj->priv_key, key.c_str() );
        EC_KEY_set_private_key( obj->key_pair, obj->priv_key );

        const EC_POINT* pub_key = EC_KEY_get0_public_key( obj->key_pair );
        obj->pub_key = EC_POINT_dup(pub_key, EC_KEY_get0_group(obj->key_pair));

    }

    template< class T >
    ecdh_t( const T& type ) noexcept : obj( new _str_() ) {

        obj->key_pair = EC_KEY_new_by_curve_name(type);
                        EC_KEY_generate_key( obj->key_pair );

        obj->pub_key  = EC_KEY_get0_public_key( obj->key_pair );
        obj->priv_key = EC_KEY_get0_private_key( obj->key_pair );

    }

    string_t get_public_key( uint x = 0 ) const noexcept { 
        unsigned char *key = NULL;
        int len = i2o_ECPublicKey( obj->key_pair , &key );
        return { (char*) &key, (ulong) len };
    }

    string_t get_public_key_hex( uint x = 0 ) const noexcept {
        return buff2hex( this->get_public_key() );
    }

    string_t get_private_key() const noexcept { 
        unsigned char *key = NULL;
        int len = i2d_ECPrivateKey( obj->key_pair , &key ); 
        return { (char*) &key, (ulong) len };
    }

    string_t get_private_key_hex() const noexcept {
        return buff2hex( this->get_private_key() );
    }

    void force_close() const noexcept { 
        if( obj->priv_key != nullptr ) BN_free( obj->priv_key );
    //  if( obj->key_pair != nullptr ) EC_KEY_free( obj->key_pair );
        if( obj->pub_key  != nullptr ) EC_POINT_free( obj->pub_key );
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~ecdh_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};  }

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

}   }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class ecdsa_t {
protected:

    struct _str_ {
        EC_GROUP *key_group = nullptr;
        EC_POINT *pub_key   = nullptr;
        BIGNUM   *priv_key  = nullptr;
        EC_KEY   *key_pair  = nullptr;
    };  ptr_t<_str_> obj;
    
public:

    template< class T >
    ecdsa_t( const string_t& key, const T& type ) noexcept : obj( new _str_() ) {

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
    ecdsa_t( const T& type ) noexcept : obj( new _str_() ) {

        obj->key_pair  = EC_KEY_new();
        obj->key_group = EC_GROUP_new_by_curve_name( type );

        EC_KEY_set_group( obj->key_pair, obj->key_group );
        EC_KEY_generate_key( obj->key_pair );

        obj->pub_key  = (EC_POINT*) EC_KEY_get0_public_key( obj->key_pair );
        obj->priv_key = (BIGNUM*) EC_KEY_get0_private_key( obj->key_pair );
    }

    string_t get_public_key( uint x = 0 ) const noexcept {
        string_t inp = get_public_key_hex(x), out; while( !inp.empty() ){
            string_t sec = inp.splice(0,2); char ch = 0; 
            string::parse( sec, "%02x", &ch );
            out.push( ch );
        }   return out;
    }

    string_t get_private_key() const noexcept {
        string_t inp = get_private_key_hex(), out; while( !inp.empty() ){
            string_t sec = inp.splice(0,2); char ch = 0; 
            string::parse( sec, "%02x", &ch );
            out.push( ch );
        }   return out;
    }

    string_t get_public_key_hex( uint x = 0 ) const noexcept { 
        point_conversion_form_t y; switch( x ){
            case 0:  y = POINT_CONVERSION_HYBRID;       break;
            case 1:  y = POINT_CONVERSION_COMPRESSED;   break;
            default: y = POINT_CONVERSION_UNCOMPRESSED; break;
        }   return EC_POINT_point2hex( obj->key_group, obj->pub_key, y, nullptr );
    }

    string_t get_private_key_hex() const noexcept { return BN_bn2hex( obj->priv_key ); }

    void force_close() const noexcept { 
        if( obj->priv_key  != nullptr ) BN_free( obj->priv_key );
    //  if( obj->key_pair  != nullptr ) EC_KEY_free( obj->key_pair );
        if( obj->pub_key   != nullptr ) EC_POINT_free( obj->pub_key );
        if( obj->key_group != nullptr ) EC_GROUP_free( obj->key_group );
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~ecdsa_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};  }

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

}   }

/*────────────────────────────────────────────────────────────────────────────*/
/* DH & rsa falta OK
namespace crypto { class rsa_t {
protected:

    struct _str_ {
        RSA    *rsa = nullptr;
        BIGNUM *key = nullptr;
        uint    len = 512;
    };  ptr_t<_str_> obj;
    
public:

    template< class T >
    rsa_t( uint size ) noexcept : obj( new _str_() ) {
        obj->len = size;
        obj->key = BN_new();
        obj->rsa = RSA_new();
        BN_set_word( obj->key, RSA_F4 );
    }

    string_t get_key( uint x = 0 ) const noexcept {
        return hex2buff( this->get_key_hex() );
    }

    string_t get_key_hex( uint x = 0 ) const noexcept { 
        if ( RSA_generate_key_ex( obj->rsa, obj->len, obj->key, NULL ) != 1 ) 
           { return ""; } BN_bn2hex( obj->key ); return obj->key;
    }

    void force_close() const noexcept { 
        if( obj->rsa != nullptr ) RSA_free( obj->rsa );
        if( obj->key != nullptr ) BN_free( obj->key );
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~rsa_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};  }
*/
/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class dsa_t {
protected:

    struct _str_ {
        DSA    *dsa = nullptr;
        uint    len = 512;
    };  ptr_t<_str_> obj;
    
public:

    template< class T >
    dsa_t( uint size ) : obj( new _str_() ) {
        obj->len = size; obj->dsa = DSA_new();
        if( DSA_generate_parameters_ex( obj->dsa, obj->len, NULL, 0, NULL, NULL, NULL ) != 1 )
          { _Error("while generating DSA parameters"); }
        if( DSA_generate_key( obj->dsa ) != 1 )
          { _Error("while generating DSA key"); }

    }

    template< class T >
    dsa_t( string_t path, uint size ) : obj( new _str_() ) {
        obj->len = size; obj->dsa = DSA_new(); FILE* fp = fopen(path.c_str(),"r");
        if ( fp == nullptr ) _Error("such file or directory does not exist");
        obj->dsa = PEM_read_DSAPrivateKey( fp, &obj->dsa, nullptr, nullptr );
        fclose( fp ); if( obj->dsa==nullptr ) _Error("while creating DSA");
    }

    string_t sign( string_t hash, uint hash_digest ) const noexcept {
        uchar signature[DSA_size(obj->dsa)]; uint len;
        DSA_sign( 0, (uchar*)hash.c_str(), hash_digest, signature, &len, obj->dsa );
        return { (char*)signature, (ulong) len };
    }

    void save_private_key( string_t path ) const {
        FILE* fp = fopen( path.c_str(), "w" );
        if ( fp == nullptr ) { _Error("while creating file"); }
        if ( PEM_write_DSA_PUBKEY( fp, obj->dsa ) != 1 ) 
           { fclose( fp ); _Error("while writting the private key"); } fclose( fp );
    }

    void save_public_key( string_t path ) const {
        FILE* fp = fopen( path.c_str(), "w" );
        if ( fp == nullptr ) { _Error("while creating file"); }
        if ( PEM_write_DSAPrivateKey( fp, obj->dsa, nullptr, nullptr, 0, nullptr, nullptr ) != 1 )
           { fclose( fp ); _Error("while writting the public key"); } fclose( fp );
    }

    void force_close() const noexcept { 
        if( obj->dsa != nullptr ) DSA_free( obj->dsa );
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~dsa_t() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};  }

namespace crypto { namespace sign {
    
    class DSA : public ecdsa_t { public: template< class... T >
          DSA ( const T&... args ) noexcept : dsa_t( args... ) {}
    };

} }

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif

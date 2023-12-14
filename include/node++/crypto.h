#ifndef NODEPP_CRYPTO
#define NODEPP_CRYPTO

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

namespace nodepp {

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

    void update( const string_t& msg ) const noexcept { EVP_DigestUpdate( obj->ctx, (uchar*) msg.data(), msg.size() ); }

    string_t get() const noexcept { force_close(); return { (char*) &obj->buffer, obj->length }; }

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

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto {

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

}

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

    void update( const string_t& msg ) const noexcept { HMAC_Update( obj->ctx, (uchar*) msg.data(), msg.size() ); }

    string_t get() const noexcept { force_close(); return { (char*) &obj->buffer, obj->length }; }

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

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto {

    class HMAC_MD4 : public hmac_t { public:
          HMAC_MD4 ( const string_t& key ) noexcept : hmac_t( key, EVP_md4(), MD4_DIGEST_LENGTH ) {}
    };

    class HMAC_MD5 : public hmac_t { public:
          HMAC_MD5 ( const string_t& key ) noexcept : hmac_t( key, EVP_md5(), MD5_DIGEST_LENGTH ) {}
    };

    class HMAC_SHA1 : public hmac_t { public:
          HMAC_SHA1 ( const string_t& key ) noexcept : hmac_t( key, EVP_sha1(), SHA_DIGEST_LENGTH ) {}
    };

    class HMAC_SHA256 : public hmac_t { public:
          HMAC_SHA256 ( const string_t& key ) noexcept : hmac_t( key, EVP_sha256(), SHA256_DIGEST_LENGTH ) {}
    };

    class HMAC_SHA384 : public hmac_t { public:
          HMAC_SHA384 ( const string_t& key ) noexcept : hmac_t( key, EVP_sha384(), SHA384_DIGEST_LENGTH ) {}
    };

    class HMAC_SHA512 : public hmac_t { public:
          HMAC_SHA512 ( const string_t& key ) noexcept : hmac_t( key, EVP_sha512(), SHA512_DIGEST_LENGTH ) {}
    };

}

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
        obj->bff    = ptr_t<uchar>(UNBFF_SIZE,0);
        obj->ctx    = EVP_CIPHER_CTX_new(); 
        obj->state  = 1; 
        EVP_EncryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), (uchar*)iv.data() );
    }

    void update( const string_t& msg ) const noexcept {
        EVP_EncryptUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size());
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    string_t get() const noexcept { force_close(); return obj->buffer; }

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
        obj->bff    = ptr_t<uchar>(UNBFF_SIZE,0);
        obj->ctx    = EVP_CIPHER_CTX_new(); 
        obj->state  = 1;
        EVP_EncryptInit_ex( obj->ctx, type, NULL, (uchar*)key.data(), (uchar*)iv.data() );
    }

    void update( const string_t& msg ) const noexcept {
        EVP_DecryptUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size());
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    string_t get() const noexcept { force_close(); return obj->buffer; }

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

namespace crypto { namespace enc {
    
    class AES_X128_CBC : public encrypt_t { public:
          AES_X128_CBC( const string_t& iv, const string_t& key ) noexcept : encrypt_t( iv, key, EVP_aes_128_cbc() ) {}
    };
    
    class AES_X192_CBC : public encrypt_t { public:
          AES_X192_CBC( const string_t& iv, const string_t& key ) noexcept : encrypt_t( iv, key, EVP_aes_192_cbc() ) {}
    };
    
    class AES_X256_CBC : public encrypt_t { public:
          AES_X256_CBC( const string_t& iv, const string_t& key ) noexcept : encrypt_t( iv, key, EVP_aes_256_cbc() ) {}
    };

    /*─······································································─*/
    
    class AES_X128_ECB : public encrypt_t { public:
          AES_X128_ECB( const string_t& iv, const string_t& key ) noexcept : encrypt_t( iv, key, EVP_aes_128_ecb() ) {}
    };
    
    class AES_X192_ECB : public encrypt_t { public:
          AES_X192_ECB( const string_t& iv, const string_t& key ) noexcept : encrypt_t( iv, key, EVP_aes_192_ecb() ) {}
    };
    
    class AES_X256_ECB : public encrypt_t { public:
          AES_X256_ECB( const string_t& iv, const string_t& key ) noexcept : encrypt_t( iv, key, EVP_aes_256_ecb() ) {}
    };

    /*─······································································─*/
    
    class DES_CBC : public encrypt_t { public:
          DES_CBC ( const string_t& iv, const string_t& key ) noexcept : encrypt_t( iv, key, EVP_des_cbc() ) {}
    };
    
    class DES_ECB : public encrypt_t { public:
          DES_ECB ( const string_t& iv, const string_t& key ) noexcept : encrypt_t( iv, key, EVP_des_ecb() ) {}
    };

    /*─······································································─*/
    
    class RC4 : public encrypt_t { public:
          RC4 ( const string_t& iv, const string_t& key ) noexcept : encrypt_t( iv, key, EVP_rc4() ) {}
    };

}   }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace dec {
    
    class AES_X128_CBC : public decrypt_t { public:
          AES_X128_CBC( const string_t& iv, const string_t& key ) noexcept : decrypt_t( iv, key, EVP_aes_128_cbc() ) {}
    };
    
    class AES_X192_CBC : public decrypt_t { public:
          AES_X192_CBC( const string_t& iv, const string_t& key ) noexcept : decrypt_t( iv, key, EVP_aes_192_cbc() ) {}
    };
    
    class AES_X256_CBC : public decrypt_t { public:
          AES_X256_CBC( const string_t& iv, const string_t& key ) noexcept : decrypt_t( iv, key, EVP_aes_256_cbc() ) {}
    };

    /*─······································································─*/
    
    class AES_X128_ECB : public decrypt_t { public:
          AES_X128_ECB( const string_t& iv, const string_t& key ) noexcept : decrypt_t( iv, key, EVP_aes_128_ecb() ) {}
    };
    
    class AES_X192_ECB : public decrypt_t { public:
          AES_X192_ECB( const string_t& iv, const string_t& key ) noexcept : decrypt_t( iv, key, EVP_aes_192_ecb() ) {}
    };
    
    class AES_X256_ECB : public decrypt_t { public:
          AES_X256_ECB( const string_t& iv, const string_t& key ) noexcept : decrypt_t( iv, key, EVP_aes_256_ecb() ) {}
    };

    /*─······································································─*/
    
    class DES_CBC : public decrypt_t { public:
          DES_CBC ( const string_t& iv, const string_t& key ) noexcept : decrypt_t( iv, key, EVP_des_cbc() ) {}
    };
    
    class DES_ECB : public decrypt_t { public:
          DES_ECB ( const string_t& iv, const string_t& key ) noexcept : decrypt_t( iv, key, EVP_des_ecb() ) {}
    };

    /*─······································································─*/
    
    class RC4 : public decrypt_t { public:
          RC4 ( const string_t& iv, const string_t& key ) noexcept : decrypt_t( iv, key, EVP_rc4() ) {}
    };

}   }

/*────────────────────────────────────────────────────────────────────────────*/

} namespace nodepp { 

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace enc { class BASE64 {
protected:

    struct _str_ {
        EVP_ENCODE_CTX* ctx; 
        ptr_t<uchar> bff;
        string_t buffer;
        int state, len;
    };  ptr_t<_str_> obj;
    
public:

    BASE64() noexcept : obj( new _str_() ) {
        obj->bff    = ptr_t<uchar>(UNBFF_SIZE,0);
        obj->ctx    = EVP_ENCODE_CTX_new();
        obj->state  = 1;
        EVP_EncodeInit( obj->ctx );
    }

    void update( const string_t& msg ) const noexcept {
        EVP_EncodeUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size()); 
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    string_t get() const noexcept { force_close(); return obj->buffer; }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_EncodeFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_ENCODE_CTX_free( obj->ctx ); //EVP_cleanup();
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~BASE64() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};  }}

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace dec { class BASE64 {
protected:

    struct _str_ {
        EVP_ENCODE_CTX* ctx; 
        ptr_t<uchar> bff;
        string_t buffer;
        int state, len;
    };  ptr_t<_str_> obj;

public:

    BASE64() noexcept : obj( new _str_() ) {
        obj->bff    = ptr_t<uchar>(UNBFF_SIZE,0);
        obj->ctx    = EVP_ENCODE_CTX_new();
        obj->state  = 1;
        EVP_DecodeInit( obj->ctx );
    }

    void update( const string_t& msg ) const noexcept {
        EVP_DecodeUpdate( obj->ctx, &obj->bff, &obj->len, (uchar*)msg.data(), msg.size()); 
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    string_t get() const noexcept { force_close(); return obj->buffer; }

    void force_close() const noexcept { 
        if( obj->state == 0 ){ return; } obj->state = 0;
        EVP_DecodeFinal( obj->ctx, &obj->bff, &obj->len ); 
        EVP_ENCODE_CTX_free( obj->ctx ); //EVP_cleanup();
        obj->buffer += (string_t){ (char*)&obj->bff, (ulong) obj->len };
    }

    void free() const noexcept { force_close(); } 
    
    virtual ~BASE64() noexcept { 
        if( obj.count()>1 ){ return; } 
            force_close();
    }

};  }}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif

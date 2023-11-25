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

#include "node++/stream.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class hash_t {
protected:
    EVP_MD_CTX* ctx; ptr_t<int> state;
    unsigned int length;
    ptr_t<uchar> buffer;
public:

    template< class T >
    hash_t( T type, ulong length ) noexcept : ctx(EVP_MD_CTX_new()), state(new int(1)) { 
        EVP_DigestInit_ex( ctx, type, NULL );
        buffer = ptr_t<uchar>( length );
    }

    void update( string_t msg ) noexcept { EVP_DigestUpdate( ctx, (uchar*) msg.c_str(), msg.size() ); }

    string_t get() noexcept { return { (char*) &buffer, buffer.size() }; }

    string_t done() noexcept { force_close(); return get(); }

    operator string_t(){ return done(); } 

    void force_close() noexcept {
        if( *state == 0 ){ return; } *state = 0;
        EVP_DigestFinal_ex( ctx, &buffer, &length );
        EVP_MD_CTX_free( ctx ); EVP_cleanup();
    }

    void free() noexcept { force_close(); } 

    virtual ~hash_t() noexcept { 
        if( state.count()>1 ){ return; } 
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
    HMAC_CTX* ctx; ptr_t<int> state;
    ptr_t<uchar> buffer;
public:

    template< class T >
    hmac_t( string_t key, T type, ulong length ) noexcept : ctx(HMAC_CTX_new()), state(new int(1)) { 
        HMAC_Init_ex( ctx, key.c_str(), key.size(), type, nullptr );
        buffer = ptr_t<uchar>( length );
    }

    void update( string_t msg ) noexcept { HMAC_Update( ctx, (uchar*) msg.c_str(), msg.size() ); }

    string_t get() noexcept { return { (char*) &buffer, buffer.size() }; }

    string_t done() noexcept { force_close(); return get(); }

    operator string_t(){ return done(); } 

    void force_close() noexcept {
        if( *state == 0 ){ return; } *state = 0;
        HMAC_Final( ctx, &buffer, nullptr ); 
        HMAC_CTX_free( ctx ); EVP_cleanup();
    }

    void free() noexcept { force_close(); } 
    
    virtual ~hmac_t() noexcept { 
        if( state.count()>1 ){ return; } 
            force_close();
    }
};  }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto {

    class HMAC_MD4 : public hmac_t { public:
          HMAC_MD4 ( string_t key ) noexcept : hmac_t( key, EVP_md4(), MD4_DIGEST_LENGTH ) {}
    };

    class HMAC_MD5 : public hmac_t { public:
          HMAC_MD5 ( string_t key ) noexcept : hmac_t( key, EVP_md5(), MD5_DIGEST_LENGTH ) {}
    };

    class HMAC_SHA1 : public hmac_t { public:
          HMAC_SHA1 ( string_t key ) noexcept : hmac_t( key, EVP_sha1(), SHA_DIGEST_LENGTH ) {}
    };

    class HMAC_SHA256 : public hmac_t { public:
          HMAC_SHA256 ( string_t key ) noexcept : hmac_t( key, EVP_sha256(), SHA256_DIGEST_LENGTH ) {}
    };

    class HMAC_SHA384 : public hmac_t { public:
          HMAC_SHA384 ( string_t key ) noexcept : hmac_t( key, EVP_sha384(), SHA384_DIGEST_LENGTH ) {}
    };

    class HMAC_SHA512 : public hmac_t { public:
          HMAC_SHA512 ( string_t key ) noexcept : hmac_t( key, EVP_sha512(), SHA512_DIGEST_LENGTH ) {}
    };

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class encrypt_t {
protected:
    EVP_CIPHER_CTX* ctx; ptr_t<int> state; 
    int len; string_t buffer;
public:

    template< class T >
    encrypt_t( string_t iv, string_t key, T type ) noexcept : ctx(EVP_CIPHER_CTX_new()), state(new int(1)) {
        EVP_EncryptInit_ex( ctx, type, NULL, (uchar*)key.c_str(), (uchar*)iv.c_str() );
    }

    void update( string_t msg ) noexcept { uchar bff[msg.size()];
        EVP_EncryptUpdate( ctx, bff, &len, (uchar*)msg.c_str(), msg.size()); 
        buffer += (string_t){ (char*)bff, (ulong) len };
    }

    string_t done() noexcept { force_close(); return get(); }

    string_t get() noexcept { return buffer; }

    operator string_t(){ return done(); } 

    void force_close() noexcept { 
        if( *state == 0 ){ return; } 
        *state = 0; uchar bff[UNBFF_SIZE];
        EVP_EncryptFinal( ctx, bff, &len ); 
        EVP_CIPHER_CTX_free( ctx ); EVP_cleanup();
        buffer += (string_t){ (char*)bff, (ulong) len };
    }

    void free() noexcept { force_close(); } 
    
    virtual ~encrypt_t() noexcept { 
        if( state.count()>1 ){ return; } 
            force_close();
    }

};  }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { class decrypt_t {
protected:
    EVP_CIPHER_CTX* ctx; ptr_t<int> state; 
    int len; string_t buffer;
public:

    template< class T >
    decrypt_t( string_t iv, string_t key, T type ) noexcept : ctx(EVP_CIPHER_CTX_new()), state(new int(1)) {
        EVP_DecryptInit_ex( ctx, type, NULL, (uchar*)key.c_str(), (uchar*)iv.c_str() );
    }

    void update( string_t msg ) noexcept { uchar bff[msg.size()];
        EVP_DecryptUpdate( ctx, bff, &len, (uchar*)msg.c_str(), msg.size()); 
        buffer += (string_t){ (char*)bff, (ulong) len };
    }

    string_t done() noexcept { force_close(); return get(); }

    string_t get() noexcept { return buffer; }

    operator string_t(){ return done(); } 

    void force_close() noexcept { 
        if( *state == 0 ){ return; } 
        *state = 0; uchar bff[UNBFF_SIZE];
        EVP_DecryptFinal( ctx, bff, &len ); 
        EVP_CIPHER_CTX_free( ctx ); EVP_cleanup();
        buffer += (string_t){ (char*)bff, (ulong) len };
    }

    void free() noexcept { force_close(); } 
    
    virtual ~decrypt_t() noexcept { 
        if( state.count()>1 ){ return; } 
            force_close();
    }

};  }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace enc {
    
    class AES_X128_CBC : public encrypt_t { public:
          AES_X128_CBC( string_t iv, string_t key ) noexcept : encrypt_t( iv, key, EVP_aes_128_cbc() ) {}
    };
    
    class AES_X192_CBC : public encrypt_t { public:
          AES_X192_CBC( string_t iv, string_t key ) noexcept : encrypt_t( iv, key, EVP_aes_192_cbc() ) {}
    };
    
    class AES_X256_CBC : public encrypt_t { public:
          AES_X256_CBC( string_t iv, string_t key ) noexcept : encrypt_t( iv, key, EVP_aes_256_cbc() ) {}
    };

    /*─······································································─*/
    
    class AES_X128_ECB : public encrypt_t { public:
          AES_X128_ECB( string_t iv, string_t key ) noexcept : encrypt_t( iv, key, EVP_aes_128_ecb() ) {}
    };
    
    class AES_X192_ECB : public encrypt_t { public:
          AES_X192_ECB( string_t iv, string_t key ) noexcept : encrypt_t( iv, key, EVP_aes_192_ecb() ) {}
    };
    
    class AES_X256_ECB : public encrypt_t { public:
          AES_X256_ECB( string_t iv, string_t key ) noexcept : encrypt_t( iv, key, EVP_aes_256_ecb() ) {}
    };

    /*─······································································─*/
    
    class DES_CBC : public encrypt_t { public:
          DES_CBC ( string_t iv, string_t key ) noexcept : encrypt_t( iv, key, EVP_des_cbc() ) {}
    };
    
    class DES_ECB : public encrypt_t { public:
          DES_ECB ( string_t iv, string_t key ) noexcept : encrypt_t( iv, key, EVP_des_ecb() ) {}
    };

    /*─······································································─*/
    
    class RC4 : public encrypt_t { public:
          RC4 ( string_t iv, string_t key ) noexcept : encrypt_t( iv, key, EVP_rc4() ) {}
    };

}   }

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace dec {
    
    class AES_X128_CBC : public decrypt_t { public:
          AES_X128_CBC( string_t iv, string_t key ) noexcept : decrypt_t( iv, key, EVP_aes_128_cbc() ) {}
    };
    
    class AES_X192_CBC : public decrypt_t { public:
          AES_X192_CBC( string_t iv, string_t key ) noexcept : decrypt_t( iv, key, EVP_aes_192_cbc() ) {}
    };
    
    class AES_X256_CBC : public decrypt_t { public:
          AES_X256_CBC( string_t iv, string_t key ) noexcept : decrypt_t( iv, key, EVP_aes_256_cbc() ) {}
    };

    /*─······································································─*/
    
    class AES_X128_ECB : public decrypt_t { public:
          AES_X128_ECB( string_t iv, string_t key ) noexcept : decrypt_t( iv, key, EVP_aes_128_ecb() ) {}
    };
    
    class AES_X192_ECB : public decrypt_t { public:
          AES_X192_ECB( string_t iv, string_t key ) noexcept : decrypt_t( iv, key, EVP_aes_192_ecb() ) {}
    };
    
    class AES_X256_ECB : public decrypt_t { public:
          AES_X256_ECB( string_t iv, string_t key ) noexcept : decrypt_t( iv, key, EVP_aes_256_ecb() ) {}
    };

    /*─······································································─*/
    
    class DES_CBC : public decrypt_t { public:
          DES_CBC ( string_t iv, string_t key ) noexcept : decrypt_t( iv, key, EVP_des_cbc() ) {}
    };
    
    class DES_ECB : public decrypt_t { public:
          DES_ECB ( string_t iv, string_t key ) noexcept : decrypt_t( iv, key, EVP_des_ecb() ) {}
    };

    /*─······································································─*/
    
    class RC4 : public decrypt_t { public:
          RC4 ( string_t iv, string_t key ) noexcept : decrypt_t( iv, key, EVP_rc4() ) {}
    };

}   }

/*────────────────────────────────────────────────────────────────────────────*/

} namespace nodepp { 

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace enc { class BASE64 {
protected:
    EVP_ENCODE_CTX* ctx; ptr_t<int> state; 
    int len; string_t buffer;
public:

    BASE64() noexcept : ctx(EVP_ENCODE_CTX_new()), state(new int(1)) {
        EVP_EncodeInit( ctx );
    }

    void update( string_t msg ) noexcept { uchar bff[msg.size()];
        EVP_EncodeUpdate( ctx, bff, &len, (uchar*)msg.c_str(), msg.size()); 
        buffer += (string_t){ (char*)bff, (ulong) len };
    }

    string_t done() noexcept { force_close(); return get(); }

    string_t get() noexcept { return buffer; }

    operator string_t(){ return done(); } 

    void force_close() noexcept { 
        if( *state == 0 ){ return; } 
        *state = 0; uchar bff[UNBFF_SIZE];
        EVP_EncodeFinal( ctx, bff, &len ); 
        EVP_ENCODE_CTX_free( ctx ); EVP_cleanup();
        buffer += (string_t){ (char*)bff, (ulong) len };
    }

    void free() noexcept { force_close(); } 
    
    virtual ~BASE64() noexcept { 
        if( state.count()>1 ){ return; } 
            force_close();
    }

};  }}

/*────────────────────────────────────────────────────────────────────────────*/

namespace crypto { namespace dec { class BASE64 {
protected:
    EVP_ENCODE_CTX* ctx; ptr_t<int> state; 
    int len; string_t buffer;
public:

    BASE64() noexcept : ctx(EVP_ENCODE_CTX_new()), state(new int(1)) {
        EVP_DecodeInit( ctx );
    }

    void update( string_t msg ) noexcept { uchar bff[msg.size()];
        EVP_DecodeUpdate( ctx, bff, &len, (uchar*)msg.c_str(), msg.size()); 
        buffer += (string_t){ (char*)bff, (ulong) len };
    }

    string_t done() noexcept { force_close(); return get(); }

    string_t get() noexcept { return buffer; }

    operator string_t(){ return done(); } 

    void force_close() noexcept { 
        if( *state == 0 ){ return; } 
        *state = 0; uchar bff[UNBFF_SIZE];
        EVP_DecodeFinal( ctx, bff, &len ); 
        EVP_ENCODE_CTX_free( ctx ); EVP_cleanup();
        buffer += (string_t){ (char*)bff, (ulong) len };
    }

    void free() noexcept { force_close(); } 
    
    virtual ~BASE64() noexcept { 
        if( state.count()>1 ){ return; } 
            force_close();
    }

};  }}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif

#include <nodepp/nodepp.h>
#include <nodepp/encoder.h>
#include <nodepp/crypto.h>

using namespace nodepp;

void onMain(){

    crypto::sign::DH key1, key2, key3;

    auto skey1 = key1.get_public_key();
    auto skey2 = key2.get_public_key();
    auto skey3 = key3.get_public_key();

    auto sign1 = key2.sign( skey1 );
    auto sign2 = key1.sign( skey2 );

    console::log( "verified", key1.verify( sign1, sign2 ) );
    console::log( "verified", key2.verify( sign2, sign1 ) );

    console::log( "verified", key1.verify( sign1, skey3 ) );
    console::log( "verified", key2.verify( sign2, skey3 ) );

}
#include <nodepp/nodepp.h>
#include <nodepp/crypto.h>

using namespace nodepp;

void onMain(){

    crypto::rsa::RSA ppt; ppt.generate_key();

    auto data = ppt.private_encrypt("Hello World!");
    console::log( data.size() );

    auto decp = ppt.public_decrypt( data );
    console::log( decp.size(), decp );

}
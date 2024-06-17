#include <nodepp/nodepp.h>
#include <nodepp/crypto.h>

using namespace nodepp;

void onMain(){

    crypto::enc::RSA ppt;
    string_t msg = "Hello World!";

    auto data = ppt.private_encrypt( msg );
    console::log( data.size(), msg );

    auto decp = ppt.public_decrypt( data );
    console::log( decp.size(), decp );

}
#include <node++/node++.h>
#include <node++/tls.h>

using namespace nodepp;

ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

void _Ready() {

    auto client = tls::client( &ssl );

    client.onOpen([=]( socket_t cli ){
    
        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose.once([=](){
            console::log("closed");
        });

        cli.write("hola mundo");

    });

    client.connect( "localhost", 8000, []( socket_t cli ){
        console::log("client started at https://localhost:8000");
    });

}
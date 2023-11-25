#include <node++/node++.h>
#include <node++/tls.h>

using namespace nodepp;

ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

void _Ready() {

    auto server = tls::server( &ssl );

    server.onConnect([=]( ssocket_t cli ){

        console::log("connected to:", cli.get_fd());
    
        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose([=](){
            console::log("closed");
        });

    });

    server.listen( "localhost", 8000, []( ssocket_t srv ){
        console::log("server started at https://localhost:8000");
    });

}
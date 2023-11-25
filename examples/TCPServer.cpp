#include <node++/node++.h>
#include <node++/tcp.h>

using namespace nodepp;

void _Ready() {

    auto server = tcp::server();

    server.onConnect([=]( socket_t cli ){
    
        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose([=](){
            console::log("closed");
        });

    });

    server.listen( "localhost", 8000, []( socket_t srv ){
        console::log("server started at http://localhost:8000");
    });

}
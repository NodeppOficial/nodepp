#include <node++/node++.h>
#include <node++/timer.h>
#include <node++/ws.h>

using namespace nodepp;

void _Ready() { 

    auto server = ws::server();

    server.onConnect([]( socket_t cli ){

        console::log("connected");

        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose([=](){
            console::log("closed");
        });

    });

    server.listen( "localhost", 8000, []( ... ){
        console::log("server started at http://localhost:8000");
    });

}
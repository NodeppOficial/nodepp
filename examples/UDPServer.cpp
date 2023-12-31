#include <node++/node++.h>
#include <node++/date.h>
#include <node++/udp.h>

using namespace nodepp;

void server( int process ){

    auto server = udp::server([=]( socket_t cli ){ 
        console::log( "cli:", cli.read() );
        cli.write("hola cliente\n");
    });

    server.listen( "localhost", 8000, []( socket_t server ){
        console::log("-> server started");
    });

}

void $Ready() { server( os::pid() ); }
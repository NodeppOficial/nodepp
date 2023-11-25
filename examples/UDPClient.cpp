#include <node++/node++.h>
#include <node++/date.h>
#include <node++/udp.h>

using namespace nodepp;

void client( int process ){

    auto client = udp::client([=]( socket_t srv ){
        srv.write("mojon de mierda");
        console::log( "srv:", srv.read() );
    });

    client.connect( "localhost", 8000, []( socket_t client ){
        console::log("-> client started");
    });

}

void _Ready() { client( os::pid() ); }
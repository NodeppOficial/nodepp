#include <node++/node++.h>
#include <node++/tcp.h>

using namespace nodepp;

void _Ready() {

    auto client = tcp::client();

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
        console::log("client started at http://localhost:8000");
    });

}
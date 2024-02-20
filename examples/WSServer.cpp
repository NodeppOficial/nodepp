#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/ws.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

onMain([](){

    auto server = ws::server();

    server.onConnect([]( ws_t cli ){

        console::log("connected");

        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose([=](){
            console::log("closed");
        });

    });

    server.listen( "localhost", 8000, []( ... ){
        console::log("server started at ws://localhost:8000");
    });

})

/*────────────────────────────────────────────────────────────────────────────*/
#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/fs.h>
#include <nodepp/ws.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;
file_t cin;

/*────────────────────────────────────────────────────────────────────────────*/

void server() {

    auto server = ws::server();

    server.onConnect([]( ws_t cli ){

        console::log("connected");

        cli.onData([=]( string_t chunk ){
            console::log( "server:", chunk );
        });

        cli.onClose([=](){
            console::log("closed");
        });
        
        cin.onData([=]( string_t data ){
            cli.write( data );
        });

    });

    server.listen( "localhost", 8000, []( ... ){
        console::log("server started at ws://localhost:8000");
    });

}

/*────────────────────────────────────────────────────────────────────────────*/

void client() {

    auto cli = ws::client( "ws://localhost:8000/" );
    
    cli.onOpen([=](){ 
        
        console::log("connected"); 

        cli.onData([]( string_t chunk ){ 
            console::log("client:>",chunk); 
        });

        cli.onClose([](){ 
            console::log("closed"); 
            process::exit(1);
        });
        
        cin.onData([=]( string_t data ){
            cli.write( data );
        });

    });

    cli.onError([=]( except_t err ){
        console::log(err);
    });

}

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {
    cin = fs::std_input();

    if( process::env::get("mode") == "client" )
        client(); else server();

    stream::pipe( cin );
}

/*────────────────────────────────────────────────────────────────────────────*/
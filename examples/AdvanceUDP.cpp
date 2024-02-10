#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/udp.h>
#include <nodepp/fs.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;
file_t cin;

/*────────────────────────────────────────────────────────────────────────────*/

void server() {

    auto server = udp::server();

    server.onConnect([]( socket_t cli ){

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
        console::log("server started at udp://localhost:8000");
    });

}

/*────────────────────────────────────────────────────────────────────────────*/

void client() {

    auto client = udp::client();
    
    client.onOpen([=]( socket_t cli ){ 
        
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

    client.connect( "localhost", 8000, []( ... ){
        console::log("client connected to udp://localhost:8000");
    });

}

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {
    cin = fs::stdin();

    if( process::env::get("mode") == "client" )
        client(); else server();

    stream::pipe( cin );
}

/*────────────────────────────────────────────────────────────────────────────*/
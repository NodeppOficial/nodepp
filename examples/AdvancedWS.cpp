#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/fs.h>
#include <nodepp/ws.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

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

        process::add([=]( file_t inp ){
            string_t bff ( UNBFF_SIZE, '\0' ); int c=0;
        _Start

            while( cli.is_available() ){
              if ((c=inp._read( bff.data(), bff.size() )) == -2 ){ _Next; continue; }
                cli.write({ bff.data(), (ulong)c });
            }   inp.close();
        
        _Stop
        }, fs::cin() );

    });

    server.listen( "localhost", 8000, []( ... ){
        console::log("server started at ws://localhost:8000");
    });

}

/*────────────────────────────────────────────────────────────────────────────*/

void client() {

    auto client = ws::client( "ws://localhost:8000/" );
    
    client.onOpen([=](){ 
        
        console::log("connected"); 

        client.onData([]( string_t chunk ){ 
            console::log("client:>",chunk); 
        });

        client.onClose([](){ 
            console::log("closed"); 
        });

        process::add([=]( file_t inp ){
            string_t bff ( UNBFF_SIZE, '\0' ); int c=0;
        _Start

            while( client.is_available() ){
              if ((c=inp._read( bff.data(), bff.size() )) == -2 ){ _Next; continue; }
                client.write({ bff.data(), (ulong)c });
            }   inp.close();
        
        _Stop
        }, fs::cin() );

    });

    client.onError([=]( except_t err ){
        console::log(err);
    });

}

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {
    if( process::env::get("mode") == "client" )
        client(); else server();
}

/*────────────────────────────────────────────────────────────────────────────*/
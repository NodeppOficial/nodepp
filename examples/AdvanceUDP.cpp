#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/udp.h>
#include <nodepp/fs.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

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

    client.connect( "localhost", 8000, []( ... ){
        console::log("client connected to udp://localhost:8000");
    });

}

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {
    if( process::env::get("mode") == "client" )
        client(); else server();
}

/*────────────────────────────────────────────────────────────────────────────*/
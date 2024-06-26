#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/wss.h>
#include <nodepp/fs.h>

using namespace nodepp;

void onMain() {

    ssl_t ssl( "./ssl/cert.key", "./ssl/cert.crt" );

    auto cli = wss::client( "wss://localhost:8000/", &ssl );
    
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
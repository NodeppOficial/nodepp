#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/fs.h>
#include <nodepp/ws.h>

using namespace nodepp;

void onMain() {

    auto cli = ws::client( "ws://localhost:8000/" );
    auto cin = fs::std_input();
    
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

    stream::pipe( cin );

}
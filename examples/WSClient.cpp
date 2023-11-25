#include <node++/node++.h>
#include <node++/timer.h>
#include <node++/ws.h>

using namespace nodepp;

void _Ready() {

    auto client = ws::client( "http://localhost:8000/" );
    
    client.onOpen([=](){ 
        
        console::log("connected"); 

        timer::add([=](){ 
            if( client.is_closed() ){ return -1; } static int x = 0; x++; 
            client.write( string::format("hola mundo %d",x) );
            return 1;
        },1000);

        client.onClose([](){ console::log("closed"); });

    });

    client.onError([=]( except_t err ){
        console::log(err);
    });

}
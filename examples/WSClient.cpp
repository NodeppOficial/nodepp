#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/ws.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

onMain([](){

    auto client = ws::client( "ws://localhost:8000/" );
    
    client.onOpen([=](){ 
        
        console::log("connected"); 

        client.write("Hello World!");

        client.onClose([](){ console::log("closed"); });

    });

    client.onError([=]( except_t err ){
        console::log(err);
    });

})

/*────────────────────────────────────────────────────────────────────────────*/
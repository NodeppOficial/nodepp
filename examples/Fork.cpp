#include <node++/node++.h>
#include <node++/timer.h>
#include <node++/fork.h>

using namespace nodepp;

void $Ready() { 

    auto x = fork_t( "./main" );

    if( process::is_child() ){

        x.onData([]( string_t chunk ){
            console::log( chunk );
        });

        x.onClose([](){
            console::log("client closed");
        });

    } else {

        timer::add([=](){
            static int y = 0; y++;
            auto data = string::format("hola %d",y);
            return x.write(data) > 0 ? 1 : -1;
        },1000);

        timer::timeout([=](){
            x.close();
        },10000);

        x.onClose([](){
            console::log("server closed");
        });

    }

}
#include <node++/node++.h>
#include <node++/timer.h>
#include <node++/observer.h>

using namespace nodepp;

observer_t obj ({
    { "a", 0 },
    { "b", 0 },
});

void _Ready(){

    obj.on( "b", []( any_t A, any_t B ){
        console::done( " B:>", (int)A, "|", (int)B );
    });

    obj.on( "a", []( any_t A, any_t B ){
        console::error( "A:>", (int)A, "|", (int)B );
    });
    
    timer::interval([=](){ obj.set( "b", rand() % 10 ); },1000);
    timer::interval([=](){ obj.set( "a", rand() % 10 ); },5000);

}
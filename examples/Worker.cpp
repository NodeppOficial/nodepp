#include <nodepp/nodepp.h>
#include <nodepp/worker.h>
#include <nodepp/timer.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    worker::add([](){
        console::log("Hello World");
        worker::delay( 100 ); 
        return 1;
    });

    timer::interval([=](){
        console::done("interval");
    },1000);

}

/*────────────────────────────────────────────────────────────────────────────*/
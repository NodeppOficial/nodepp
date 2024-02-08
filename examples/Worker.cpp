#include <nodepp/nodepp.h>
#include <nodepp/worker.h>
#include <nodepp/timer.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    ptr_t<int> x = new int(100);
    mutex_t mut;

    worker::add([=](){
    coStart

        while( *x > 0 ){
            mut.lock(); *x-=1;
            console::info("Hello World",*x);
            worker::delay( 100 );
            mut.unlock(); coNext;
        }

    coStop
    });

    worker::add([=](){
    coStart

        while( *x > 0 ){
            mut.lock(); *x-=1;
            console::done("Hello World",*x);
            worker::delay( 100 );
            mut.unlock(); coNext;
        }

    coStop
    });

}

/*────────────────────────────────────────────────────────────────────────────*/
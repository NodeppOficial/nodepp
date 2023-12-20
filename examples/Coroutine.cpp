#include <node++/node++.h>

using namespace nodepp;

void $Ready() {

    process::loop::add([=](){ static int itr = 10;
    $Start
        while( itr --> 0 ){
            console::done(" Coroutine 1:",itr); $Next;
        }
    $Stop
    });

    process::loop::add([=](){ static int itr = 10;
    $Start
        while( itr --> 0 ){
            console::error("Coroutine 2:",itr); $Next;
        }
    $Stop
    });

}

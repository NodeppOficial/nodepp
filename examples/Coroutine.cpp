#include <node++/node++.h>

using namespace nodepp;

void _Ready() {

    process::loop::add([=](){ static int itr = 10;
    _Start
        while( itr --> 0 ){
            console::done(" Coroutine 1:",itr); _Yield(1);
        }
    _Stop
    });

    process::loop::add([=](){ static int itr = 10;
    _Start
        while( itr --> 0 ){
            console::error("Coroutine 2:",itr); _Yield(1);
        }
    _Stop
    });

}

#include <node++/node++.h>
#include <node++/worker.h>

using namespace nodepp;

void _Ready() {

    worker::add([=](){ static int itr = 10;
    _Start
        while( itr --> 0 ){
            console::done(" Process 1:",itr); _Yield(1);
        }
    _Stop
    });

    worker::add([=](){ static int itr = 10;
    _Start
        while( itr --> 0 ){
            console::error("Process 2:",itr); _Yield(1);
        }
    _Stop
    });

}

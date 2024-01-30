#include <nodepp/nodepp.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    process::add([=](){ static int itr = 10;
    _Start
        while( itr --> 0 ){
            console::done(" Coroutine 1:",itr); _Next;
        }
    _Stop
    });

    process::add([=](){ static int itr = 10;
    _Start
        while( itr --> 0 ){
            console::error("Coroutine 2:",itr); _Next;
        }
    _Stop
    });

}

/*────────────────────────────────────────────────────────────────────────────*/

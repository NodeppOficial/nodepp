#include <nodepp/nodepp.h>
#include <nodepp/timer.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    auto A = timer::interval([=](){ 
        static int i=0; i++; i %= 1000;
        console::done(" interval every: 1 second - ",i," seconds");
    },1000);

    auto B = timer::timeout([=](){
        console::error("timeout after: 10 seconds ");
    },10000);

    timer::clear(A); timer::clear(B);

}

/*────────────────────────────────────────────────────────────────────────────*/
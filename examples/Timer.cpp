#include <nodepp/nodepp.h>
#include <nodepp/timer.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void onMain(){

    //Interval: a setInterval equivalent
    auto A = timer::interval([=](){ 
        static int i=0; i++; i %= 1000;
        console::done(" interval every: 1 second - ",i," seconds");
    },1000);

    //Timeout: a setTimeout equivalent
    auto B = timer::timeout([=](){
        console::error("timeout after: 10 seconds ");
    },10000);

    //Clear the timers
    //NOTE: this code will not work until delete the next line
    timer::clear(A); timer::clear(B);

}

/*────────────────────────────────────────────────────────────────────────────*/
#include <nodepp/nodepp.h>
#include <nodepp/timer.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void onMain(){
    
    timer::add([](){
        static int i = 0; i++;
        console::log("interval every: 1 second - ",i," seconds");
        return ( i >= 10 ) ? -1 : 1;
    }, 1000 );

}

/*────────────────────────────────────────────────────────────────────────────*/
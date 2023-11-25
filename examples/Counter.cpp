#include <node++/node++.h>
#include <node++/timer.h>

using namespace nodepp;

void _Ready() { console::start(9600);

    timer::add([](){
        static int i = 0; i++;
        console::log("interval every: 1 second - ",i," seconds");
        return ( i >= 10 ) ? -1 : 1;
    }, 1000 );

}
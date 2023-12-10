#include <node++/node++.h>
#include <node++/simkey.h>
#include <node++/timer.h>

using namespace nodepp;

#define VELOCITY .05

ptr_t<float> vec ({ 0, 0 });

void $Ready() { 

    timer::interval([](){
        control::mouse::set_position( vec[0], vec[1] );
    },1);

    timer::interval([](){

        static float x = 0; x += VELOCITY;

        vec[0] = 50 + sin( x * 1 ) * 20;
        vec[1] = 50 + cos( x * 2 ) * 20;

    },100);

    timer::timeout([](){
        process::exit(1);
    },100000);

}
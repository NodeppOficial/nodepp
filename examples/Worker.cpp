#include <node++/node++.h>
#include <node++/worker.h>
#include <node++/timer.h>

using namespace nodepp;

void _Ready() {

    worker::add([]( int arg, string_t str ){
        console::log( str, arg );
        worker::delay( 100 ); 
        return 1;
    },100,"hola mundo");

    timer::interval([=](){
        console::done("interval");
    },1000);

}
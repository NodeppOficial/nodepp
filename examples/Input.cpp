#include <node++/node++.h>
#include <node++/input.h>
#include <node++/timer.h>

using namespace nodepp;

void $Ready() { 

    input_t inp ();
    inp.set_mouse_position( 0, 0 );

}
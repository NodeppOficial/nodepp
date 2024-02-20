#include <nodepp/nodepp.h>
#include <nodepp/input.h>
#include <nodepp/timer.h>

using namespace nodepp;

input_t inp;

onMain([](){

    inp.set_mouse_position( 0, 0 );

})
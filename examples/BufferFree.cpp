#include <node++/node++.h>
#include <node++/timer.h>

using namespace nodepp;

void _Ready() {

    ptr_t<int> var = new int(10);

    if( !var.null() ) console::log( *var, &var );
    else              console::log( "undefined" );

    console::log("free"); var.free();

    if( !var.null() ) console::log( *var, &var );
    else              console::log( "undefined" );

}
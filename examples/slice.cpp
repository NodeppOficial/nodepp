#include <node++/node++.h>

using namespace nodepp;

void _Ready() { 
    
    string_t str = "0123456789";
    
    console::log( "slice", str.slice(0,5) );
    console::log( "slice", str.slice(  5) );

    console::log("----");

    console::log( "splice", str.splice(0,5) );
    console::log( "splice", str );

}
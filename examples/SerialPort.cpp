#include <node++/node++.h>
#include <node++/stream.h>
#include <node++/serial.h>

using namespace nodepp;

void _Ready(){

    auto str = serial::connect( "/dev/ttyUSB0", 9600 );

    str.onData.on([=]( string_t chunk ){
        console::print( (char*) chunk );
    });

    stream::pipeline( str );
}
#include <nodepp/nodepp.h>
#include <nodepp/stream.h>
#include <nodepp/serial.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    auto str = serial::connect( "/dev/ttyUSB0", 9600 );

    str.onData.on([=]( string_t chunk ){
        console::print( (char*) chunk );
    });

    stream::pipeline( str );
}

/*────────────────────────────────────────────────────────────────────────────*/
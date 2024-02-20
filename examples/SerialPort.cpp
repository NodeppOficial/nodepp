#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/serial.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

onMain([](){

    auto list = serial::get_devices();

    if( list.empty() ){ 
        process::error("Not Serial Device Found"); 
    } else {
        console::log( list[0] );
    }

    auto x = serial::connect( list[0] );

    x.onConnect([]( serial_t cli ){

        console::log( "connected" );

        cli.onData([]( string_t chunk ){
            console::log( chunk );
        });

    });


})

/*────────────────────────────────────────────────────────────────────────────*/
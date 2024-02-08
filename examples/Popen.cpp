#include <nodepp/nodepp.h>
#include <nodepp/popen.h>
#include <nodepp/stream.h>

using namespace nodepp;

void _main_() {

    auto p = popen::async( "curl", { "curl", "http://www.google.com/" } );

    p.onDout([]( string_t chunk ){
        conio::done("stdout: ");
        console::log( chunk );
    });

    p.onDerr([]( string_t chunk ){
        conio::error("stderr: ");
        console::log( chunk );
    });

    p.onData([]( string_t chunk ){
        console::log(":>", chunk );
    });

}
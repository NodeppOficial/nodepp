#include <nodepp/nodepp.h>
#include <nodepp/popen.h>

using namespace nodepp;

void onMain(){

    array_t<string_t> cmd ({ "curl", "http://www.google.com/", "-Ls" });
    auto p = popen::async( cmd[0], cmd );

    p.onDout([]( string_t chunk ){
        conio::done("stdout: ");
        console::log( chunk );
    });

    p.onDerr([]( string_t chunk ){
        conio::error("stderr: ");
        console::log( chunk );
    });

    p.onData([]( string_t chunk ){
        console::log( ":>", chunk );
    });

}
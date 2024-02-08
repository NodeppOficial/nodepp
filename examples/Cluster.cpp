#include <nodepp/nodepp.h>
#include <nodepp/cluster.h>
#include <nodepp/popen.h>
#include <nodepp/stream.h>

using namespace nodepp;

void _main_() {

    auto p = cluster::add({ "A", "B", "C" });

    if( process::is_parent() ){

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

    } else {
        for( auto x : process::args )
             p.write( x );
             p.werror("nice");
    }

}
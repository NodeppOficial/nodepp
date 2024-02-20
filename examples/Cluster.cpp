#include <nodepp/nodepp.h>
#include <nodepp/cluster.h>

using namespace nodepp;

onMain([](){

    auto p = cluster::add({ "hello ", "world ", "dude " });

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
        for( auto x : process::args ){
             p.write( x );
        }    p.werror("nice");
    }

})
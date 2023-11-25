#include <node++/node++.h>
#include <node++/fetch.h>

using namespace nodepp;

void _Ready(){

    fetch::http({
        .url = "http://www.google.com/",
    })

    .then([]( auto cli ){
        console::log( cli.headers["Host"] );
        cli.onData([]( string_t chunk ){
            console::log( chunk.size(), ":>", chunk );
        }); stream::pipe( cli );
    })

    .fail([]( auto err ){
        console::error( err );
    });

}
#include <node++/node++.h>
#include <node++/fetch.h>

using namespace nodepp;

ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

void _Ready(){

    fetch::https({
        .ssl = &ssl,
        .url = "https://localhost:8000/404",
    })

    .then([]( auto cli ){
        console::log( cli.headers["Host"] );
        cli.onData([]( string_t chunk ){
            console::log( chunk.size(), ":>", chunk );
        });
        stream::rawpipe( cli );
    })

    .fail([]( auto err ){
        console::error( err );
    });

}
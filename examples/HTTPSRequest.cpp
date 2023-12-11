#include <node++/node++.h>
#include <node++/https.h>

using namespace nodepp;

ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

void $Ready(){

    https::fetch({
        .ssl = &ssl,
        .url = "https://www.google.com/",
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
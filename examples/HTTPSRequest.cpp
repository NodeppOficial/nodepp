#include <nodepp/nodepp.h>
#include <nodepp/https.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

    fetch_t args;
            args.url = "https://www.google.com/";

    https::fetch( args, &ssl )

    .then([]( http_t cli ){
        cli.onData([]( string_t chunk ){
            console::log( chunk.size(), ":>", chunk );
        }); stream::pipe( cli );
    })

    .fail([]( except_t err ){
        console::error( err );
    });

}

/*────────────────────────────────────────────────────────────────────────────*/
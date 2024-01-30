#include <nodepp/nodepp.h>
#include <nodepp/http.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    fetch_t args;
            args.url = "http://www.google.com/";

    http::fetch( args )

    .then([]( http_t cli ){
        console::log( cli.headers["Host"] );
        cli.onData([]( string_t chunk ){
            console::log( chunk );
        }); stream::pipe( cli );
    })

    .fail([]( except_t err ){
        console::error( err );
    });

}

/*────────────────────────────────────────────────────────────────────────────*/
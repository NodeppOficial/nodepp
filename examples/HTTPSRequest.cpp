#include <nodepp/nodepp.h>
#include <nodepp/https.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

    fetch_t args;
            args.method = "GET";
            args.url = "https://www.google.com/";
            args.headers = {{
                { "Host", url::host(args.url) }
            }};
        //  args.file = file_t("PATH","r");
        //  args.body = "MYBODY";

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
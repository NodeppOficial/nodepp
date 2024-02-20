#include <nodepp/nodepp.h>
#include <nodepp/https.h>
#include <nodepp/date.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

onMain([](){

    ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

    auto server = https::server([=]( https_t cli ){

        console::log( cli.path, cli.get_fd() );
        
        cli.write_header( 200, {{
            { "content-type", "text/html" }
        }});
        
        cli.write( date::fulltime() );
        cli.close();

    }, &ssl );

    server.listen( "localhost", 8000, [=]( ssocket_t server ){
        console::log("server started at https://localhost:8000");
    });

})

/*────────────────────────────────────────────────────────────────────────────*/
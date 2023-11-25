#include <node++/node++.h>
#include <node++/https.h>
#include <node++/date.h>

using namespace nodepp;

ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

void server( int process ){

    auto server = https::server([=]( https_t cli ){

        console::log( cli.path, cli.get_fd() );
        
        cli.write_headers( 200, {{
            { "content-type", "text/html" }
        }});
        
        cli.write( date::fulltime() );
        cli.close();

    }, &ssl );

    server.listen( "localhost", 8000, [=]( ssocket_t server ){
        console::log("server started at https://localhost:8000");
    });

}

void _Ready() { server( os::pid() ); }
#include <node++/node++.h>
#include <node++/http.h>
#include <node++/date.h>

using namespace nodepp;

void server( int process ){

    auto server = http::server([=]( http_t cli ){ 

        console::log( cli.path, cli.get_fd() );
        
        cli.write_headers( 200, {{
            { "content-type", "text/html" }
        }});
        
        cli.write( date::fulltime() );
        cli.close();

    });

    server.listen( "localhost", 8000, [=]( socket_t server ){
        console::log("server started at http://localhost:8000");
    });

}

void _Ready() { server( os::pid() ); }
#include <nodepp/nodepp.h>
#include <nodepp/http.h>
#include <nodepp/date.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

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

/*────────────────────────────────────────────────────────────────────────────*/
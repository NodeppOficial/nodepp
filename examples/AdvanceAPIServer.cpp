#include <nodepp/nodepp.h>
#include <nodepp/http.h>
#include <nodepp/date.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void server() {

    auto server = http::server([=]( http_t cli ){ 

        console::log( cli.path, cli.get_fd() );

        for( auto x : cli.query ){
             console::log( "->",x.first, ":", x.second );
        }

        if( cli.method == "POST" ){
            for( auto x : cli.headers ){
                 console::log( x.first, ":", x.second );
            }    console::log( "cli:", cli.read() );
            cli.write_headers( 200, {{
                { "content-type", "text/html" }
            }}); cli.write("recived");
                 cli.close(); return;
        }

        elif ( cli.path == "/day" ){
            cli.write_headers( 200, {{
                { "content-type", "text/html" }
            }}); cli.write(string::to_string( date::day() ));
                 cli.close(); return;
        }

        elif ( cli.path == "/month" ){
            cli.write_headers( 200, {{
                { "content-type", "text/html" }
            }}); cli.write(string::to_string( date::month() ));
                 cli.close(); return;
        }

        elif ( cli.path == "/year" ){
            cli.write_headers( 200, {{
                { "content-type", "text/html" }
            }}); cli.write(string::to_string( date::year() ));
                 cli.close(); return;
        }

        else {
            cli.write_headers( 200, {{
                { "content-type", "text/html" }
            }}); cli.write( date::fulltime() );
                 cli.close(); return;
        }

    });

    server.listen( "localhost", 8000, [=]( socket_t /*unused*/ ){
        console::log("server started at http://localhost:8000");
    });

}

/*────────────────────────────────────────────────────────────────────────────*/

void client() {

    fetch_t data;
    data.method = "POST";
    data.body   = "Hello World!";
    data.url    = "http://localhost:8000/";

    http::fetch( data )

    .then([]( auto cli ){
        console::log( "srv:>", cli.read() );
    })

    .fail([]( auto err ){
        console::error( err );
    });

}

/*────────────────────────────────────────────────────────────────────────────*/

onMain([](){

    if( process::env::get("mode") == "client" )
        client(); else server();

})

/*────────────────────────────────────────────────────────────────────────────*/
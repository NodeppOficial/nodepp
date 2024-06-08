#include <nodepp/nodepp.h>
#include <nodepp/http.h>
#include <nodepp/date.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void server() {

    auto server = http::server([=]( http_t cli ){ 

        console::log( cli.path, cli.get_fd() );

        for( auto x : cli.query.data() ){
             console::log( "->",x.first, ":", x.second );
        }

        if( cli.method == "POST" ){

            if( cli.headers["content-length"].empty() ){
                cli.write_header( 404, header_t({
                    { "content-type", "text/plain" }
                }));
                cli.write("something went wrong");
                cli.close(); return;
            }

            auto data = cli.read();
            console::log( "cli:", data, data.size() );

            cli.write_header( 200, header_t({
                { "content-type", "text/html" }
            })); 
            cli.write("recived"); 
            cli.close(); return;
            
        }

        elif ( cli.path == "/day" ){
            cli.write_header( 200, header_t({
                { "content-type", "text/html" }
            })); 
            cli.write(string::to_string( date::day() ));
            cli.close(); return;
        }

        elif ( cli.path == "/month" ){
            cli.write_header( 200, header_t({
                { "content-type", "text/html" }
            })); 
            cli.write(string::to_string( date::month() ));
            cli.close(); return;
        }

        elif ( cli.path == "/year" ){
            cli.write_header( 200, header_t({
                { "content-type", "text/html" }
            })); 
            cli.write(string::to_string( date::year() ));
            cli.close(); return;
        }

        else {
            cli.write_header( 200, header_t({
                { "content-type", "text/html" }
            })); 
            cli.write( date::fulltime() );
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

void onMain(){

    if( process::env::get("mode") == "client" )
        client(); else server();

}

/*────────────────────────────────────────────────────────────────────────────*/
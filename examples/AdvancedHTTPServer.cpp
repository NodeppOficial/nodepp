#include <node++/node++.h>
#include <node++/timer.h>
#include <node++/http.h>
#include <node++/path.h>
#include <node++/date.h>
#include <node++/fs.h>

using namespace nodepp;

void server( int process ){

    auto server = http::server([=]( http_t cli ){ 

        string_t dir = "www/index.html";
        if( cli.path.size() > 1 )
            dir = path::join( "www", cli.path );

        console::log( cli.path, process );

        if( !fs::exists_file(dir) ){
            cli.write_headers( 404, {{ { "content-type", "text/plain" } }} );
            cli.write( regex::format("404: Oops time: ${0}",date::fulltime()) ); 
            cli.close(); return;
        }

        auto str = fs::readable( dir );

        if( cli.headers["Range"].empty() ){

            cli.write_headers( 200, {{
                { "Content-Length", string::to_string(str.size()) },
            //  { "Cache-Control", "public, max-age=3600" },
                { "Content-Type",   path::mimetype(dir) }
            }});

            if(!regex::test(path::mimetype(dir),"audio|video","i") ) 
                stream::pipe( str, cli );

        } else if( !cli.headers["Range"].empty() ) {

            array_t<string_t> range = regex::match_all(cli.headers["Range"],"\\d+","i");
            ulong rang[2]; rang[0] = string::to_ulong( range[0] );
                  rang[1] = min( rang[0]+CHUNK_MB(10), str.size()-1 );

            cli.write_headers( 206, {{
                { "Content-Range", regex::format("bytes ${0}-${1}/${2}",rang[0],rang[1],str.size()) },
                { "Content-Type",  path::mimetype(dir) }, 
                { "Accept-Range", "bytes" }
            }});

            str.set_range( rang[0], rang[1] );
            stream::pipe( str, cli );

        }

    });

    server.listen( "localhost", 8000, [=]( socket_t server ){
        console::log("server started at http://localhost:8000");
    });

}

void _Ready() { server( os::pid() ); }
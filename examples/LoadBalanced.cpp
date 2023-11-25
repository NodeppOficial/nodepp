#include <node++/node++.h>
#include <node++/timer.h>
#include <node++/http.h>
#include <node++/path.h>
#include <node++/date.h>
#include <node++/fs.h>

using namespace nodepp;

void server( int process ){

    auto server = http::server([=]( http_t cli ){ 
        //HTTP Logic Inside
    });

    server.listen( "localhost", 8000, [=]( socket_t server ){
        //Optional Callbac
    });

}

void _Ready() {

    //Load Balanced Web Server
    for( auto x=os::cpus(); x--; ){
        if( os::fork() != 0 ){ break; }
            server( os::pid() );
    }

}
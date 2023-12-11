#include <node++/node++.h>

#include <node++/cluster.h>
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

void $Ready() {

    if( process::is_child() ){ server( os::pid() ); } else {
        for( auto x = os::cpus(); x--; ){
             auto y = cluster_t();
        }
    }

}
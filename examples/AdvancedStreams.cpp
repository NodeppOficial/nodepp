#include <node++/node++.h>
#include <node++/path.h>
#include <node++/fs.h>

using namespace nodepp;

void _Ready() { 
    auto _rdb = fs::readable( path::join( process::cwd(), "www", "kloud.mp4" ) );
    auto _wrt = fs::writable( path::join( process::cwd(), "www", "mojon.mp4" ) );

    _rdb.onData.on([]( string_t chunk ){
        console::log( chunk.size() );
    });

    _rdb.onClose.on([](){
        console::log( "done" );
    });

    stream::pipe( _rdb, _wrt );
}
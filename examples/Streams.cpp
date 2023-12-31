#include <node++/node++.h>
#include <node++/path.h>
#include <node++/fs.h>

using namespace nodepp;

void $Ready() { 
    string_t dir = path::join( os::cwd(), "LICENSE" );
    auto _str = fs::readable( dir );

    _str.onData.on([]( string_t chunk ){
        console::log( chunk );
    });

    stream::pipe(_str);
}
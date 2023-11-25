#include <node++/node++.h>
#include <node++/path.h>

using namespace nodepp;

void _Ready() {
	
    string_t dir = path::join( process::cwd(), "www", "index.html" );
    console::log( dir );

    auto ppt = path::parse( dir );

    console::log( "root:", ppt.root );
    console::log( "path:", ppt.path );
    console::log( "name:", ppt.name );
    console::log( "base:", ppt.base );
    console::log( "extn:", ppt.ext );
    console::log( "dirn:", ppt.dir );

}
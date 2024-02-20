#include <nodepp/nodepp.h>
#include <nodepp/path.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

onMain([](){
	
    string_t dir = path::join( os::cwd(), "www", "index.html" );
    auto     ppt = path::parse( dir );

    console::log( "mime:", path::mimetype(ppt) );
    console::log( "root:", ppt.root );
    console::log( "path:", ppt.path );
    console::log( "name:", ppt.name );
    console::log( "base:", ppt.base );
    console::log( "extn:", ppt.ext );

})

/*────────────────────────────────────────────────────────────────────────────*/
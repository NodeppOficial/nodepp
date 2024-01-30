#include <nodepp/nodepp.h>
#include <nodepp/path.h>
#include <nodepp/fs.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {
    
    string_t dir = path::join( os::cwd(), "LICENSE" );
    auto    _str = fs::readable( dir );

    _str.onData.on([]( string_t chunk ){
        console::log( chunk );
    });

    stream::pipe(_str);
    
}

/*────────────────────────────────────────────────────────────────────────────*/
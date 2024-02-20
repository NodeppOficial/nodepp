#include <nodepp/nodepp.h>
#include <nodepp/path.h>
#include <nodepp/os.h>
#include <nodepp/fs.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

onMain([](){
    
    auto _rdb = fs::readable( path::join( os::cwd(), "www", "kloud.mp4" ) );
    auto _wrt = fs::writable( path::join( os::cwd(), "www", "mojon.mp4" ) );

    _rdb.onData.on([]( string_t chunk ){
        console::log( chunk.size() );
    });

    _rdb.onClose.on([](){
        console::log( "done" );
    });

    stream::pipe( _rdb, _wrt );

})

/*────────────────────────────────────────────────────────────────────────────*/
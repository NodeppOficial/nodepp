#include <nodepp/nodepp.h>
#include <nodepp/crypto.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    auto ppt = crypto::enc::AES_X192_CBC( "key1234567890" );
         ppt.update("Hello World!");
    auto p = ppt.get();

    console::log( ":>", p.size(), p );

    auto ttp = crypto::dec::AES_X192_CBC( "key1234567890" );
         ttp.update( p );
    auto q = ttp.get();

    console::log( ":>", q.size(), q );

}

/*────────────────────────────────────────────────────────────────────────────*/
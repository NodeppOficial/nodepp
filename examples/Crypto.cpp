#include <node++/node++.h>
#include <node++/crypto.h>

using namespace nodepp;

void _Ready() {

    auto ppt = crypto::enc::DES_CBC( "mojon", "mojon" );
         ppt.update(" mojon ");
         ppt.update(" de ");
         ppt.update(" mierda ");
    auto p = ppt.done();

    console::log( ":>", p.size(), p );

    auto ttp = crypto::dec::DES_CBC( "mojon", "mojon" );
         ttp.update( p );
    auto q = ttp.done();

    console::log( ":>", q.size(), q );

}
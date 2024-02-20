#include <nodepp/nodepp.h>
#include <nodepp/optional.h>

using namespace nodepp;

onMain([](){

    optional_t<string_t> x ();
//  expected_t<string_t> x ("error");

    if( x.has_value() ) console::log( x.value() );
    else                console::log( "x is empty" );

})
#include <nodepp/nodepp.h>
#include <nodepp/optional.h>

using namespace nodepp;

onMain([](){

    optional_t<string_t> x;
//  optional_t<string_t> x ("error");

    if( x.has_value() ) console::done( x.value() );
    else                console::error( "x is empty" );

})
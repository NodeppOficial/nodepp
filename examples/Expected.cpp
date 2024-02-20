#include <nodepp/nodepp.h>
#include <nodepp/expected.h>

using namespace nodepp;

onMain([](){

    expected_t<int,string_t> x ( 10 );
//  expected_t<int,string_t> x ("error");

    if( x.has_value() ) console::log( x.value() );
    else                console::log( x.error() );

})
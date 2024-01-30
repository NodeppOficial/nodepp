#include <nodepp/nodepp.h>
#include <nodepp/tuple.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    tuple_t<int,float,string_t> tp ( 10, 10.50, "hello world!" );

    console::log( tuple::get<0>(tp) );
    console::log( tuple::get<1>(tp) );
    console::log( tuple::get<2>(tp) );    

}

/*────────────────────────────────────────────────────────────────────────────*/
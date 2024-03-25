#include <nodepp/nodepp.h>
#include <nodepp/reflect.h>

using namespace nodepp;

struct object : public reflect_t {

    int data = 10;
    string_t name = "hola mundo";

    object(){
        this->reflect_field( "name", name );
        this->reflect_field( "data", data );
    }

};

onMain([](){

    object ppt;

    console::log("\n -- keys -- \n");

    console::log( ppt.get_field<int>("data") );
    console::log( ppt.get_field<string_t>("name") );

    ppt.data = 50000;
    ppt.name = "adios mundo";

    console::log("\n -- keys -- \n");

    console::log( ppt.get_field<int>("data") );
    console::log( ppt.get_field<string_t>("name") );

    console::log("\n -- keys -- \n");

    ppt.keys([]( string_t& data ){
        console::log( data );
    });

})
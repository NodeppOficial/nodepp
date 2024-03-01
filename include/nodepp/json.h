#ifndef NODEPP_JSON
#define NODEPP_JSON

/*────────────────────────────────────────────────────────────────────────────*/

#include "object.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { using json_t = object_t; }

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace json {

string_t format( const object_t& json ){
    return nullptr;
}

json_t parse( const string_t& json ){ 
    return nullptr;
}

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_JSON
#define NODEPP_JSON

/*────────────────────────────────────────────────────────────────────────────*/

#include "object.h"
#include "regex.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class json_t {
protected:

    struct NODE {
        object_t obj;
    };  ptr_t<NODE> obj;

public:

    json_t() : obj( new NODE() ){}
   ~json_t() noexcept {} 

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace json {

object_t parse( const string_t& json ){
    return nullptr;
}

string_t format( const object_t& json ){
    return nullptr;
}

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
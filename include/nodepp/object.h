#ifndef NODEPP_OBJECT
#define NODEPP_OBJECT

/*────────────────────────────────────────────────────────────────────────────*/

#include "any.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class object_t {
private:

    using ARRAY = array_t<type::pair<string_t,object_t>>;
    using T     = type::pair<string_t,object_t>;

protected: 

    struct NODE {
        uint  type;
        any_t  mem;
    }; ptr_t<NODE> obj;

public:

    template< ulong N > 
    object_t( const T (&arr) [N] ) noexcept : obj(new NODE()) { 
        ARRAY mem (N); for( ulong x=N; x--; )
        { mem[x] = arr[x]; } obj->mem = mem; 
    }

    template< class U > 
    object_t( const U& any ) noexcept : obj(new NODE()){ 
        obj->mem = any; 
    }
    
    object_t() noexcept : obj(new NODE()) {}

    /*─······································································─*/

    template< class U >
    explicit operator U() const noexcept { return obj->mem.get<U>(); }

    /*─······································································─*/

    object_t& operator[]( const string_t& name ) const noexcept {
        auto mem = obj->mem.get<ARRAY>();

        for ( ulong x=0; x<mem.size(); x++ ) {
            if( mem[x].first == string::to_string(name) )
                return mem[x].second;
        }   T item ({ name, 0 });

        mem.push( item ); obj->mem = mem; 
        return mem[mem.last()].second;
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
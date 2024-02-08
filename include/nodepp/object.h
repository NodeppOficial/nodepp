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

    uint   type = 0;
    any_t  memory;

public: object_t() noexcept {}

    template< ulong N > 
    object_t( const T (&arr) [N] ) noexcept { 
        ARRAY mem (N); for( ulong x=N; x--; )
            { mem[x] = arr[x]; } memory = mem; 
    }

    template< class U > 
    object_t( const U& any ) noexcept { memory = any; }

    /*─······································································─*/

    object_t& operator[]( const string_t& name ){

        auto mem = memory.get<ARRAY>();

        for( ulong x=0; x<mem.size(); x++ ){
            if( mem[x].first == string::to_string(name) )
                return mem[x].second;
        }   T item ({ name, 0 });

        mem.push( item ); memory = mem; 
        return mem[mem.last( )].second;
    
    }

    /*─······································································─*/

    template< class U >
    explicit operator U() const noexcept { return memory.get<U>(); }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_VARIANT
#define NODEPP_VARIANT

/*────────────────────────────────────────────────────────────────────────────*/

#include "any.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class... Types > class variant_t : public any_t {
public:

    variant_t( const char* V ) : any_t(V), idx(is_valid(V)) {}

    template< class T > 
    variant_t( const T& V ) : any_t(V), idx(is_valid(V)) {}

    variant_t() : any_t(), idx(new int(-1)) {}
    
    /*─······································································─*/

    template< class T >
    T get() const noexcept { return any_t::get<T>(); }

    template< class T > void set( const T& f ) { 
        idx = is_valid( f ); any_t::set( f );
    }
    
protected:

    template< class T >
    int* is_valid( T /*unused*/ ) { 
        int *idx = new int( get_index<T,Types...>::value );
        if (*idx > (int) sizeof...(Types) ){
             process::error("invalid data type");
        }    return idx;
    }
    
    /*─······································································─*/
    
    template< class T, class... Us > struct get_index { 
        static constexpr ulong value = 1;
    };

    template< class T, class... Us > struct get_index<T, T, Us...> { 
        static constexpr ulong value = 0;
    };

    template< class T, class U, class... Us > struct get_index<T, U, Us...> {
        static constexpr ulong value = 1 + get_index<T,Us...>::value;
    };
    
    /*─······································································─*/
    
    ptr_t<int> idx;

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
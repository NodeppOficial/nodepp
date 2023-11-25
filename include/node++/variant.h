#ifndef NODEPP_VARIANT
#define NODEPP_VARIANT

/*────────────────────────────────────────────────────────────────────────────*/

#include "any.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class... Types > class variant_t : public any_t {
public:

    template< class T > variant_t( T V ) noexcept : any_t(V), idx(new int( get_index<T,Types...>::value )) {}
       variant_t( const char* V ) noexcept : any_t(V), idx(new int( get_index<string_t,Types...>::value )) {}
       variant_t() noexcept : any_t() {}
    
    /*─······································································─*/
    
    template< class T >
    int get_type_id() const noexcept { return get_index<T,Types...>::value; }
    int get_type_id() const noexcept { return idx == nullptr ? 0 : *idx; }
    
    /*─······································································─*/

    template< class T >
    T get() const noexcept { return any_t::get<T>(); }

    template< class T > void set( T f ){ 
        idx = new int( get_index<T,Types...>::value );
        any_t::set( f );
    }
    
protected:
    
    template< class T, class... Us > struct get_index { 
        static constexpr ulong value = 1;
    };

    template< class T, class... Us > struct get_index<T, T, Us...> { 
        static constexpr ulong value = 0;
    };

    template< class T, class U, class... Us > struct get_index<T, U, Us...> {
        static constexpr ulong value = 1 + get_index<T,Us...>::value;
    };
    
    ptr_t<int> idx = new int(0);

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_OBJECT
#define NODEPP_OBJECT
#define TYPES int,uint,long,ulong,char,uchar,bool,float,double,string_t,llong,ullong,ldouble,NODE_ARRAY,NODE_OBJECT

/*────────────────────────────────────────────────────────────────────────────*/

#include "variant.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class object_t {
private:

    using NODE_OBJECT  = array_t<type::pair<string_t,object_t>>;
    using NODE_ARRAY   = array_t<object_t>;
    using T            = type::pair<string_t,object_t>;
    using V            = object_t;

protected: 

    variant_t<TYPES> memory;

    V& get( int name ){
        if( memory.get_type_id() != memory.get_type_id<NODE_ARRAY>() )
          { $Error("this node is not an Array"); }
        auto   mem = memory.get<NODE_ARRAY>();
        return mem[ (int)name % mem.size() ];
    }

    V& get( const string_t& name ){

        if( memory.get_type_id() != memory.get_type_id<NODE_OBJECT>() )
          { $Error("this node is not an Object"); }
        auto mem = memory.get<NODE_OBJECT>();

        for( ulong x=0; x<mem.size(); x++ ){
            if( mem[x].first == string::to_string(name) )
                return mem[x].second;
        }
            
        type::pair<string_t,object_t> item ({ name, 0 });
        mem.push( item ); memory = mem; 
        return mem[mem.last()].second;
    }

public: 

    template< ulong N > 
    object_t( const T (&arr) [N] ) noexcept { 
        NODE_OBJECT mem (N); for( ulong x=N; x--; )
            { mem[x] = arr[x]; } memory = mem; 
    }

    template< class U, ulong N > 
    object_t( const U (&arr) [N] ) noexcept { 
        NODE_ARRAY mem (N); for( ulong x=N; x--; )
            { mem[x] = arr[x]; } memory = mem; 
    }

    template< class U > 
    object_t( const U& any ) noexcept { memory = any; }

    object_t() noexcept { memory = (NODE_OBJECT){0,0}; }

    /*─······································································─*/

    template< class T >
    V& operator[]( const T& name ){ return get( name ); }

    /*─······································································─*/

    template< class U >
    void map( U cb ) const noexcept {

        if( get_type_id() == get_type_id<NODE_OBJECT>() ){
            auto y = 0; for( auto x : type::cast<NODE_ARRAY>(memory) ){
                 string_t j = string::to_string(y);
                 cb({ j,x }); y++;
            }
        }

        else if( get_type_id() == get_type_id<NODE_OBJECT>() ){
            for( auto x : type::cast<NODE_OBJECT>(memory) ){
                 cb({ x.first, x.second });
            }
        }

        else if( get_type_id() != -1 ){ cb({ "0", memory }); }

    }

    /*─······································································─*/

    template< class U >
    int get_type_id() const noexcept { return memory.get_type_id<U>(); }
    
    int get_type_id() const noexcept { return memory.get_type_id(); }

    template< class T >
    explicit operator T() const noexcept { return memory.get<T>(); }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
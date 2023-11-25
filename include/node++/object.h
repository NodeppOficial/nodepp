#ifndef NODEPP_OBJECT
#define NODEPP_OBJECT

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

    variant_t<int,uint,long,ulong,char,uchar,bool,float,double,string_t,llong,ullong,ldouble,NODE_ARRAY,NODE_OBJECT> memory;
  
public:

    template< class U > object_t( const U& any ) noexcept { memory = any; }

    object_t() noexcept = default;
    
    /*─······································································─*/

    template< ulong N > object_t( const T (&obj) [N] ) noexcept { 
        NODE_OBJECT mem (N); for( ulong x=N; x--; )
            { mem[x] = obj[x]; } memory = mem; 
    }

    /*─······································································─*/

    template< ulong N > object_t( const V (&arr) [N] ) noexcept { 
        NODE_ARRAY mem (N); for( ulong x=N; x--; )
            { mem[x] = arr[x]; } memory = mem; 
    }

    /*─······································································─*/

    template< class U >
    int get_type_id() const noexcept { return memory.get_type_id<U>(); }
    int get_type_id() const noexcept { return memory.get_type_id(); }

    /*─······································································─*/

    object_t& operator[]( const string_t& name ) noexcept { 
        
        auto mem = type::cast<NODE_OBJECT>( memory );

        for( ulong x=0; x<mem.size(); x++ ){
            if( mem[x].first == name )
                return mem[x].second;
        }
        
        type::pair<string_t,object_t> item ({ name, 0 });
        mem.push( item ); memory = mem; return mem[mem.last()].second;
    }

    object_t& operator[]( const int& index ) noexcept { 
        return memory.get<NODE_ARRAY>()[index];
    }

    /*─······································································─*/

    string_t stringify() const noexcept { string_t result; 

        if( get_type_id() == get_type_id<NODE_OBJECT>() ){
            for( auto x : type::cast<NODE_OBJECT>(memory) ){
                 result += string::format("%s: %s \n", x.first.data(), x.second.stringify().data() );
            }
        }

        else if( get_type_id() == get_type_id<NODE_OBJECT>() ){
            for( auto x : type::cast<NODE_ARRAY>(memory) ){
                 result += string::format("%s,", x.stringify().data() );
            }
        }

        else if( get_type_id() == get_type_id<string_t>() ) {
            result += string::to_string( (string_t) memory );
        }

        else if( get_type_id() == get_type_id<int>() ) {
            result += string::to_string( (int) memory );
        }

        else if( get_type_id() == get_type_id<bool>() ) {
            result += string::to_string( (bool) memory );
        }

        else { result += "data"; }

        return result.data();
    }

    template< class T >
    explicit operator T() const noexcept { return memory.get<T>(); }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
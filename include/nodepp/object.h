
/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_OBJECT
#define NODEPP_OBJECT

/*────────────────────────────────────────────────────────────────────────────*/

#include "any.h"
#include "map.h"
#include "initializer.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace { 

    template< class T > struct obj_type_id                      { static constexpr int value = 0x0000 |  0; };

    template<>          struct obj_type_id<int>                 { static constexpr int value = 0x0000 |  1; };
    template<>          struct obj_type_id<uint>                { static constexpr int value = 0x0000 |  2; };
    template<>          struct obj_type_id<bool>                { static constexpr int value = 0x0000 |  3; };
    template<>          struct obj_type_id<char>                { static constexpr int value = 0x0000 |  4; };
    template<>          struct obj_type_id<long>                { static constexpr int value = 0x0000 |  5; };
    template<>          struct obj_type_id<short>               { static constexpr int value = 0x0000 |  6; };
    template<>          struct obj_type_id<uchar>               { static constexpr int value = 0x0000 |  7; };
    template<>          struct obj_type_id<llong>               { static constexpr int value = 0x0000 |  8; };
    template<>          struct obj_type_id<ulong>               { static constexpr int value = 0x0000 |  9; };
    template<>          struct obj_type_id<ushort>              { static constexpr int value = 0x0000 | 10; };
    template<>          struct obj_type_id<ullong>              { static constexpr int value = 0x0000 | 11; };

    template<>          struct obj_type_id<wchar_t>             { static constexpr int value = 0x0000 | 12; };
    template<>          struct obj_type_id<char16_t>            { static constexpr int value = 0x0000 | 13; };
    template<>          struct obj_type_id<char32_t>            { static constexpr int value = 0x0000 | 14; };

    template<>          struct obj_type_id<float>               { static constexpr int value = 0x0000 | 15; };
    template<>          struct obj_type_id<double>              { static constexpr int value = 0x0000 | 16; };
    template<>          struct obj_type_id<ldouble>             { static constexpr int value = 0x0000 | 17; };
    
    template<>          struct obj_type_id<string_t>            { static constexpr int value = 0x0000 | 18; };
    template< ulong N > struct obj_type_id<char[N]>             { static constexpr int value = 0x0000 | 18; };
    template<>          struct obj_type_id<char*>               { static constexpr int value = 0x0000 | 18; };

    template< class T >          struct obj_type_id<T*>         { static constexpr int value = 0xf000 | obj_type_id<T>::value; };
    template< class T, ulong N > struct obj_type_id<T[N]>       { static constexpr int value = 0xf200 | obj_type_id<T>::value; };
    template< class T > struct obj_type_id<initializer_t<T>>    { static constexpr int value = 0xf400 | obj_type_id<T>::value; };
    template< class T > struct obj_type_id<ptr_t<T>>            { static constexpr int value = 0xf600 | obj_type_id<T>::value; };
    template< class T > struct obj_type_id<queue_t<T>>          { static constexpr int value = 0xf800 | obj_type_id<T>::value; };

    template< class T > struct obj_type_id<array_t<T>>          { static constexpr int value = 0xfA00 | obj_type_id<T>::value; };
    template< class T > struct obj_type_id<map_t<string_t,T>>   { static constexpr int value = 0xfC00 | obj_type_id<T>::value; };

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class object_t {
private:

    using T     = type::pair<string_t,object_t>;
    using ARRAY = array_t<object_t>;
    using QUEUE = queue_t<T>;

protected: 

    struct NODE {
        any_t  mem   ;
        int   type =0;
    };  ptr_t<NODE> obj;

public:

    template< ulong N > 
    object_t( const T (&arr) [N] ) noexcept : obj(new NODE()) { 
        QUEUE mem; for( ulong x=0; x<N; x++ )
            { mem.push( arr[x] ); }
        obj->mem = mem; obj->type = 20;
    }

    template< class U > 
    object_t( const U& any ) noexcept : obj(new NODE()) { 
        if( type::is_same<U,ARRAY>::value )
          { obj->type = 21; goto BACK; }  
        obj->type = obj_type_id<U>::value;
        BACK:; obj->mem  = any;
    }
    
    object_t() noexcept : obj( new NODE() ) {}

    /*─······································································─*/

    template< class U >
    explicit operator U() const noexcept { return obj->mem.as<U>(); }

    bool has_value() const noexcept { return obj->mem.has_value(); }

    uint type_size() const noexcept { return obj->mem.type_size(); }

    template< class U > 
    U as() const { 
        if ( get_type_id() < 20 && get_type_id() > 21 &&
             obj_type_id<U>::value != get_type_id()
          ){ process::error( "not valid object type" ); }
             return obj->mem.as<U>(); 
    }

    /*─······································································─*/

    object_t& operator[]( const string_t& name ) const noexcept {
        if( !has_value() || obj->type != 20  )
          { obj->mem=QUEUE(); obj->type= 20; }

        auto mem = type::cast<QUEUE>(obj->mem);
        auto x   = mem.first();

        while( x != nullptr ){ auto y = x->next;
           if( x->data.first == string::to_string(name) )
             { return x->data.second; }
           if(!x->data.second.has_value() )
             { mem.erase(x); } x = y;
        }

        T item ({ name, object_t() }); mem.push( item ); 
        obj->mem = mem; obj->type = 20;
        return mem.last()->data.second;
    }

    bool has( const string_t& name ) const noexcept {
        if( !has_value() || obj->type != 20  )
          { obj->mem=QUEUE(); obj->type= 20; }

        auto mem = type::cast<QUEUE>(obj->mem);
        auto x   = mem.first();

        while( x != nullptr ){
           if( x->data.first == string::to_string(name) )
             { return true; } x = x->next;
        }

        return false;
    }

    /*─······································································─*/

    object_t& operator[]( const ulong& idx ) const {
        if( !has_value() )
          { process::error("item is empty"); }
        auto mem = type::cast<ARRAY>(obj->mem);
        return mem[idx];
    }

    /*─······································································─*/

    bool has( const ulong& idx ) const {
        if( !has_value() )   { return false; }
        auto mem = type::cast<ARRAY>(obj->mem);
        return mem.size() >= idx;
    }

    /*─······································································─*/

    array_t<string_t> keys() const noexcept { array_t<string_t> res;
        if( obj->type == 20 ){ auto mem = obj->mem.as<QUEUE>();
            mem.map([&]( T item ){ res.push( item.first ); });
        }   return res;
    }

    int get_type_id() const noexcept { return obj->type; }

    /*─······································································─*/

    bool empty() const noexcept { 
        if( obj->type == 21 ){ 
            auto   mem = obj->mem.as<ARRAY>();
            return mem.empty();
        } elif( obj->type == 20 ) {
            auto   mem = obj->mem.as<QUEUE>();
            return mem.empty();
        }   return false;
    }

    /*─······································································─*/

    ulong size() const noexcept { 
        if( obj->type == 21 ){ 
            auto   mem = obj->mem.as<ARRAY>();
            return mem.size();
        } elif( obj->type == 20 ) {
            auto   mem = obj->mem.as<QUEUE>();
            return mem.size();
        }   return 0;
    }

    /*─······································································─*/

    void erase( const string_t& name ) const noexcept {
        if( !has_value() || obj->type != 20  )
          { obj->mem=QUEUE(); obj->type= 20; }

        auto mem = obj->mem.as<QUEUE>();
        auto x   = mem.first();

        while( x != nullptr ) { auto y = x->next;
           if( x->data.first == string::to_string(name) )
             { mem.erase( x ); } x = y;
        } 
        
    }

    void erase() noexcept { obj = new NODE(); }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

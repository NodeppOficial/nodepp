/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.nodepp.xyz/license.html
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_REFLECT
#define NODEPP_REFLECT

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class reflect_t {
protected:

    using T = type::pair< string_t, void* >;
    queue_t<T> fields;

public:

    /*─······································································─*/

    reflect_t () noexcept = default;

    /*─······································································─*/

    template < typename V >
    void reflect_field( const string_t& name, V& value ) noexcept {
         fields.push({ name, type::cast<void>( &value ) });
    }

    /*─······································································─*/

    void keys( function_t<void,string_t&> callback ) const noexcept {
         fields.map([=]( T& data ){ callback( data.first ); });
    }

    array_t<string_t> get_keys() const noexcept {
        array_t<string_t> res ( fields.size() );
        ulong n=0; keys([&]( string_t& data ){
            res[n] = data; n++;
        }); return res;
    }

    /*─······································································─*/

    template < class V, class = typename type::enable_if<!type::is_pointer<V>::value,V>::type >
    V& get_field( const string_t& fieldName ) {
        auto x = fields.index_of([=]( T data ){ return data.first == fieldName; });
        if( x<0 ){ process::error( "Field not found [",fieldName,"]"  ); }
        return *type::cast<V>( fields[x]->data.second );
    }

    template < class V, class = typename type::enable_if<type::is_pointer<V>::value,V>::type >
    V* get_field( const string_t& fieldName ) {
        auto x = fields.index_of([=]( T data ){ return data.first == fieldName; });
        if( x<0 ){ process::error( "Field not found [",fieldName,"]" ); }
        return type::cast<V>( fields[x]->data.second );
    }

    /*─······································································─*/

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
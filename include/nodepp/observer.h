/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_OBSERVER
#define NODEPP_OBSERVER

/*────────────────────────────────────────────────────────────────────────────*/

#include "event.h" 
#include "type.h"
#include "any.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class observer_t {
private:

    template< class A, class B, class C >
    struct NODE_ { A first; B second; C third; };

    using V = any_t;
    using U = string_t; 
    using E = event_t<V,V>;
    using T = NODE_<U,V,E>;
    using P = type::pair<U,V>;

protected: array_t<T> node;
public:    observer_t() noexcept {}

    template< class... O >
    observer_t( const P& argc, const O&... args ) noexcept {
        node = array_t<T>( (ulong) sizeof...(args)+1 );
        ulong x = 0; iterator::map([&]( P arg ){
            node[x].second = arg.second;
            node[x].first  = arg.first;  x++;
        }, argc, args... );
    }
    
    /*─······································································─*/

    template< ulong N >
    observer_t& operator=( const P (&args) [N] ) noexcept {
        node = array_t<T>( N ); for( ulong x=N; x--; ){ 
        node[x].second = args[x].second; 
        node[x].first  = args[x].first;
    } return *this; }

    template< ulong N >
    observer_t ( const P (&args) [N] ) noexcept { 
        node = array_t<T>( N ); for( ulong x=N; x--; ){ 
        node[x].second = args[x].second;
        node[x].first  = args[x].first; 
    }}
    
    /*─······································································─*/

    void off( void* address ) const noexcept { 
        if( !address ){ return; }
        *((int*)address) = -1; 
    }

    template< class F >
    void* once( const U& name, F func ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
        if ( node[x].first == name )
             return node[x].third.once( func );
        }    return nullptr;
    }

    template< class F >
    void* on( const U& name, F func ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
        if ( node[x].first == name )
             return node[x].third.on( func );
        }    return nullptr;
    }
    
    /*─······································································─*/
    
    void set( function_t<observer_t,observer_t> func ) const {
        observer_t obj = func( *this ); for( auto x : obj )
            this->set( x.first, x.second );
    }
    
    /*─······································································─*/

    template< class F >
    void set( const U& name, const F& value ) const {
        for( ulong x=0; x<node.size(); x++ ){
         if( node[x].first == name ){
             node[x].third.emit( node[x].second, value );
             node[x].second = value; return;
        }}   process::error("field not found:",name);
    }
    
    /*─······································································─*/
    
    template< class V, ulong N >
    void set( const V (&args) [N] ) const {
         for( ulong x=0; x<N; x++ )
              this->set( args[x].first, args[x].second );
    }

    /*─······································································─*/

    const V get( const U& name ) const {
        for( ulong x=0; x<node.size(); x++ ){
        if ( node[x].first == name )
             return node[x].second;
        }    process::error( "field not found:", name ); 
             return (const V)(0);
    }

    /*─······································································─*/

    const T* begin() const noexcept { return node.begin(); }
    const T*   end() const noexcept { return node.end();   }
    
    /*─······································································─*/

    void clear( string_t name ) const noexcept { 
        for( ulong x=0; x<node.size(); x++ ){
        if ( node[x].first == name )
           { node[x].third.clear(); }
        }
    }

    void clear() const noexcept { 
        for( ulong x=0; x<node.size(); x++ )
           { node[x].third.clear(); }
    }
    
    /*─······································································─*/

    const V operator[]( const U& name ) const {
        return get( name );
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
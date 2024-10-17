/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODE_PROMISE
#define NODE_PROMISE

/*────────────────────────────────────────────────────────────────────────────*/

#include "generator.h"
#include "expected.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace promise {

    template< class T, class V > void* resolve( 
        function_t<void,function_t<void,T>,function_t<void,V>> func,
        function_t<void,T> res, function_t<void,V> rej
    ){  
        ptr_t<bool> state = new bool(1); _promise_::resolve task;
        return process::task::add( task, state, func, [=]( T data ){
           if( *state != 1 ){ return; } res(data); *state = 0;
        }, [=]( V data ) {
           if( *state != 1 ){ return; } rej(data); *state = 0;
        } );
    }

    /*─······································································─*/

    template< class T > void* resolve( 
        function_t<void,function_t<void,T>> func,
        function_t<void,T> res
    ){  
        ptr_t<bool> state = new bool(1); _promise_::resolve task;
        return process::task::add( task, state, func, [=]( T data ){
           if( *state != 1 ){ return; } res(data); *state = 0;
        } );
    }

    /*─······································································─*/

    template< class T > T await( 
        function_t<void,function_t<void,T>> func 
    ){  
        ptr_t<bool> state = new bool(1); T result; 
        _promise_::resolve task;
        process::await( task, state, func, [&]( T data ){
           if( *state != 1 ){ return; } result = data; *state = 0;
        } ); return result;
    }

    /*─······································································─*/

    template< class T, class V > expected_t<T,V> await( 
        function_t<void,function_t<void,T>,function_t<void,V>> func 
    ){   
        ptr_t<bool> state = new bool(1); T res; V rej; bool x=0;
        _promise_::resolve task;
        process::await( task, state, func, [&]( T data ){
            if( *state != 1 ){ return; } res = data; *state = 0; x=1;
        }, [&]( V data ){
            if( *state != 1 ){ return; } rej = data; *state = 0; x=0;
        } ); if( x ){ return res; } return rej;
    }
    
    /*─······································································─*/

    void clear( void* address ){ process::clear( address ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class T, class V > class promise_t { 
protected:

    struct NODE {
        function_t<void,function_t<void,T>,function_t<void,V>> main_func;
        function_t<void,T> res_func; void* addr = nullptr;
        function_t<void,V> rej_func; int state;
    };  ptr_t<NODE> obj = new NODE();

    using slf = promise_t<T,V>;

public:

    promise_t& then( const decltype(NODE::res_func)& cb ) noexcept { obj->state=2; obj->res_func = cb; return (*this); }
    
    promise_t& fail( const decltype(NODE::rej_func)& cb ) noexcept { obj->state=2; obj->rej_func = cb; return (*this); }

    /*─······································································─*/

    void resolve() const noexcept { if( obj->state!=2 ){ return; } obj->state=0;
         obj->addr = promise::resolve<T,V>( obj->main_func, obj->res_func, obj->rej_func ); 
    }

    /*─······································································─*/

    promise_t( const decltype(NODE::main_func)& cb ) noexcept { obj->main_func = cb; obj->state = 1; }

   ~promise_t() noexcept { if( obj.count()>1 ){ return; } resolve(); }

    promise_t() noexcept { obj->state = 0; }

    /*─······································································─*/

    expected_t<T,V> await() const noexcept {
        return promise::await<T,V>( obj->main_func );
    }

    /*─······································································─*/

    void clear() const noexcept {
        process::clear( obj->addr );
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
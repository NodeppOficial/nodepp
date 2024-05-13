/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_EVENT
#define NODEPP_EVENT

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class... A > class event_t { 
protected:

    using NODE = function_t<int,A...>;
    ptr_t<queue_t<NODE>> obj;

public: event_t() noexcept : obj( new queue_t<NODE>() ) {}
    
    /*─······································································─*/

    void* operator()( function_t<void,A...> func ) const noexcept { return on(func); }
    
    /*─······································································─*/

    bool  empty() const noexcept { return obj->empty(); }
    ulong  size() const noexcept { return obj->size(); }
    void  clear() const noexcept { obj->clear(); }
    
    /*─······································································─*/

    void emit( const A&... args ) const noexcept {
        auto x = obj->first(); while( x != nullptr ){
        auto y = x->next; switch( x->data( args... ) ){
            case  0: obj->erase(x); break;
            case -1: obj->erase(x); break;
            case  1:                break;
        } x = y; }
    }
    
    /*─······································································─*/

    void off( void* address ) const noexcept { 
        if( !address ){ return; }
        *((int*)address) = -1; 
    }

    void* once( function_t<void,A...> func ) const noexcept {
        ptr_t<int> out = new int(0);
        obj->push([=]( A... args ){
            if( *out >= 0 ) func( args... );
            return *out;
        }); return &out;
    }

    void* on( function_t<void,A...> func ) const noexcept {
        ptr_t<int> out = new int(1);
        obj->push([=]( A... args ){
            if( *out >= 0 ) func( args... );
            return *out;
        }); return &out;
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
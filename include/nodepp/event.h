/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.nodepp.xyz/license.html
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_EVENT
#define NODEPP_EVENT

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class... A > class event_t { 
protected:

    struct NODE {
        function_t<void,A...> cb;
        bool                  on;
    };  ptr_t<queue_t<NODE>> obj;

public: event_t() noexcept : obj( new queue_t<NODE>() ) {}
    
    /*─······································································─*/

    void* operator()( function_t<void,A...> func ) const noexcept { return on(func); }
    
    /*─······································································─*/

    ulong empty() const noexcept { return obj->empty(); }
    ulong  size() const noexcept { return obj->size(); }
    void  clear() const noexcept { obj->clear(); }
    
    /*─······································································─*/

    void emit( const A&... args ) const noexcept {
        auto x = obj->first(); while( x != nullptr ){
             x->data.cb( args... ); if( !x->data.on )
            { auto y=x->next; obj->erase(x); x=y; } 
              else x=x->next; 
        }
    }
    
    /*─······································································─*/

    void off( void* id ) const noexcept { auto x = obj->first(); 
        while( id != nullptr && x != nullptr ){
            if ( x == id ){ obj->erase(x); break; } 
            else x = x->next; 
        }
    }

    void* once( function_t<void,A...> func ) const noexcept {
                       obj->push({ func, 0 }); 
        return (void*) obj->last();
    }

    void* on( function_t<void,A...> func ) const noexcept {
                       obj->push({ func, 1 }); 
        return (void*) obj->last();
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
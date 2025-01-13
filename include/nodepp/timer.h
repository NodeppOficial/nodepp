/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_TIMER
#define NODEPP_TIMER

/*────────────────────────────────────────────────────────────────────────────*/

#include "generator.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace timer {
    
    template< class V, class... T >
    void* add ( V func, ulong* time, const T&... args ){
        auto prs = _timer_::timer();
        return process::task::add( prs, func, time, args... ); 
    };
    
    template< class V, class... T >
    void* add ( V func, ulong time, const T&... args ){
        auto prs = _timer_::timer();
        return process::task::add( prs, func, time, args... ); 
    };
    
    /*─······································································─*/

    template< class V, class... T >
    void* timeout ( V func, ulong* time, const T&... args ){
        return timer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };

    template< class V, class... T >
    void* timeout ( V func, ulong time, const T&... args ){
        return timer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };
    
    /*─······································································─*/

    template< class V, class... T >
    void* interval ( V func, ulong* time, const T&... args ){
        return timer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };

    template< class V, class... T >
    void* interval( V func, ulong time, const T&... args ){
        return timer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };
    
    /*─······································································─*/
    
    void await( ulong* time ){
        ulong stamp = process::millis() + *time;
        while( process::millis() < stamp )
             { process::next(); }
    };

    void await( ulong time ){ await( (ulong*) &time ); }
    
    /*─······································································─*/

    void clear( void* address ){ process::clear( address ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace utimer {
    
    template< class V, class... T >
    void* add ( V func, ulong* time, const T&... args ){
        auto prs = _timer_::utimer();
        return process::task::add( prs, func, time, args... ); 
    };
    
    template< class V, class... T >
    void* add ( V func, ulong time, const T&... args ){
        auto prs = _timer_::utimer();
        return process::task::add( prs, func, time, args... ); 
    };
    
    /*─······································································─*/

    template< class V, class... T >
    void* timeout ( V func, ulong* time, const T&... args ){
        return utimer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };

    template< class V, class... T >
    void* timeout ( V func, ulong time, const T&... args ){
        return utimer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };
    
    /*─······································································─*/

    template< class V, class... T >
    void* interval ( V func, ulong* time, const T&... args ){
        return utimer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };

    template< class V, class... T >
    void* interval( V func, ulong time, const T&... args ){
        return utimer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };
    
    /*─······································································─*/
    
    void await( ulong* time ){
        ulong stamp = process::micros() + *time;
        while( process::micros() < stamp )
             { process::next(); }
    };

    void await( ulong time ){ await( (ulong*) &time ); }
    
    /*─······································································─*/

    void clear( void* address ){ process::clear( address ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

#ifndef NODEPP_TIMER
#define NODEPP_TIMER

/*────────────────────────────────────────────────────────────────────────────*/

#include "generator.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace timer {
    
    template< class V, class... T >
    ptr_t<ulong> add ( V func, ulong* time, const T&... args ){
        ptr_t<ulong> out = new ulong( process::millis() + *time ); 
        auto         prs = _timer_::timer();
        process::task::add( prs, func, out, time, args... ); 
        return out;
    };
    
    template< class V, class... T >
    ptr_t<ulong> add ( V func, ulong time, const T&... args ){
        ptr_t<ulong> out = new ulong( process::millis() + time ); 
        auto         prs = _timer_::timer();
        process::task::add( prs, func, out, time, args... ); 
        return out;
    };
    
    /*─······································································─*/

    template< class V, class... T >
    ptr_t<ulong> timeout ( V func, ulong* time, const T&... args ){
        return timer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };

    template< class V, class... T >
    ptr_t<ulong> timeout ( V func, ulong time, const T&... args ){
        return timer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };
    
    /*─······································································─*/

    template< class V, class... T >
    ptr_t<ulong> interval ( V func, ulong* time, const T&... args ){
        return timer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };

    template< class V, class... T >
    ptr_t<ulong> interval( V func, ulong time, const T&... args ){
        return timer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };
    
    /*─······································································─*/
    
    void delay( ulong* time ){
        ulong stamp = process::millis() + *time;
        while( process::millis() < stamp )
             { process::next(); }
    };

    void delay( ulong time ){ delay( (ulong*) &time ); }
    
    /*─······································································─*/

    void clear( const ptr_t<ulong>& address ){ if( !address ) *address = 0; }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace utimer {
    
    template< class V, class... T >
    ptr_t<ulong> add ( V func, ulong* time, const T&... args ){
        ptr_t<ulong> out = new ulong( process::micros() + *time );  
        auto         prs = _timer_::utimer();
        process::task::add( prs, func, out, args... ); 
        return out;
    };
    
    template< class V, class... T >
    ptr_t<ulong> add ( V func, ulong time, const T&... args ){
        ptr_t<ulong> out = new ulong( process::micros() + time );  
        auto         prs = _timer_::utimer();
        process::task::add( prs, func, out, args... ); 
        return out;
    };
    
    /*─······································································─*/

    template< class V, class... T >
    ptr_t<ulong> timeout ( V func, ulong* time, const T&... args ){
        return utimer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };

    template< class V, class... T >
    ptr_t<ulong> timeout ( V func, ulong time, const T&... args ){
        return utimer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };
    
    /*─······································································─*/

    template< class V, class... T >
    ptr_t<ulong> interval ( V func, ulong* time, const T&... args ){
        return utimer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };

    template< class V, class... T >
    ptr_t<ulong> interval( V func, ulong time, const T&... args ){
        return utimer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };
    
    /*─······································································─*/
    
    void delay( ulong* time ){
        ulong stamp = process::micros() + *time;
        while( process::micros() < stamp )
             { process::next(); }
    };

    void delay( ulong time ){ delay( (ulong*) &time ); }
    
    /*─······································································─*/

    void clear( const ptr_t<ulong>& address ){ if( !address ) *address = 0; }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

#ifndef NODEPP_TIMER
#define NODEPP_TIMER

#include "generators.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace timer {
    
    template< class V, class... T >
    ptr_t<int> add ( V func, ulong* time, T... args ){
        ptr_t<int>   out = new int(1); 
        auto         prs = _timer_::timer();
        process::task::add( prs, func, out, time, args... ); 
        return out;
    };

    template< class V, class... T >
    ptr_t<int> add ( V func, const ulong& time, T... args ){ 
        ptr_t<int>   out = new int(1); 
        auto         prs = _timer_::timer();
        process::task::add( prs, func, out, time, args... ); 
        return out;
    };
    
    /*─······································································─*/

    template< class V, class... T >
    ptr_t<int> timeout ( V func, ulong* time, T... args ){
        return timer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };

    template< class V, class... T >
    ptr_t<int> timeout ( V func, const ulong& time, T... args ){
        return timer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };
    
    /*─······································································─*/

    template< class V, class... T >
    ptr_t<int> interval ( V func, ulong* time, T... args ){
        return timer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };

    template< class V, class... T >
    ptr_t<int> interval( V func, const ulong& time, T... args ){
        return timer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };
    
    /*─······································································─*/
    
    void delay( ulong* time ){
        ulong stamp = process::millis() + *time;
        while( process::millis() < stamp )
             { process::next(); }
    };

    void delay( const ulong& time ){ delay( (ulong*) &time ); }
    
    /*─······································································─*/

    void clear( const ptr_t<int>& address ){ if( !address ) *address = 0; }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace utimer {
    
    template< class V, class... T >
    ptr_t<int> add ( V func, ulong* time, T... args ){
        ptr_t<int>   out = new int(1); 
        auto         prs = _timer_::utimer();
        process::task::add( prs, func, out, time, args... ); 
        return out;
    };

    template< class V, class... T >
    ptr_t<int> add ( V func, const ulong& time, T... args ){ 
        ptr_t<int>   out = new int(1); 
        auto         prs = _timer_::utimer();
        process::task::add( prs, func, out, time, args... ); 
        return out;
    };
    
    /*─······································································─*/

    template< class V, class... T >
    ptr_t<int> timeout ( V func, ulong* time, T... args ){
        return utimer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };

    template< class V, class... T >
    ptr_t<int> timeout ( V func, const ulong& time, T... args ){
        return utimer::add([=]( T... args ){ func(args...); return -1; }, time, args... );
    };
    
    /*─······································································─*/

    template< class V, class... T >
    ptr_t<int> interval ( V func, ulong* time, T... args ){
        return utimer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };

    template< class V, class... T >
    ptr_t<int> interval( V func, const ulong& time, T... args ){
        return utimer::add([=]( T... args ){ func(args...); return 1; }, time, args... );
    };
    
    /*─······································································─*/
    
    void delay( ulong* time ){
        ulong stamp = process::micros() + *time;
        while( process::micros() < stamp )
             { process::next(); }
    };

    void delay( const ulong& time ){ delay( (ulong*) &time ); }
    
    /*─······································································─*/

    void clear( const ptr_t<int>& address ){ if( !address ) *address = 0; }

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif

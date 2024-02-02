#ifndef NODEPP_NODEPP
#define NODEPP_NODEPP

/*────────────────────────────────────────────────────────────────────────────*/

#include "import.h"
#include "regex.h"
#include "task.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    array_t<string_t> args; int threads = 0; 

    /*─······································································─*/

    ulong size(){ 
        return process::poll::size() + 
               process::task::size() + 
               process::loop::size() + 
               process::threads      ; 
    }
    
    /*─······································································─*/

    void start( int argc, char** args ){
        int i=0; do {
            if(!regex::test(args[i],"^\\?") ) {
                process::args.push(args[i]);
            } else { auto argc = string_t( args[i] ); argc.shift(); 
                 auto data = string::split( argc, '&' );
            for( auto x : data ){ 
                 auto y = regex::match_all( x, "[^=]+" );
                 if ( y.size() != 2 ){ continue; }
                 process::env::set( y[0], y[1] ); 
            }} 
        }   while( i ++< argc - 1 );
        #if _KERNEL != NODEPP_KERNEL_ARDUINO
            process::signal::start();
        #endif
    }
    
    /*─······································································─*/

    int next(){ 
        static int x = 0;
    _Start

        x = process::task::size(); while( x-->0 ){ process::task::next(); _Next; }
        x = process::loop::size(); while( x-->0 ){ process::loop::next(); _Next; }
        x = process::poll::size(); while( x-->0 ){ process::poll::next(); _Next; }

        #if _KERNEL != NODEPP_KERNEL_ARDUINO
            process::delay( TIMEOUT );
        #endif

    _Stop
    }

    /*─······································································─*/

    template< class... T >
    void add( const T&... args ){ process::loop::add( args... ); }
    
    /*─······································································─*/

    void clear(){ 
        process::task::clear();
        process::poll::clear(); 
        process::loop::clear(); 
        process::threads = 0; 
    }
    
    /*─······································································─*/

    bool empty(){ return ( 
        process::task::empty() && 
        process::poll::empty() && 
        process::loop::empty() && 
        process::threads < 1 
    ) ; }
    
    /*─······································································─*/

    void pipe(){
        while( !process::empty() )
                process::next();
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
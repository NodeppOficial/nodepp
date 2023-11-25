#ifndef NODEPP_NODEPP
#define NODEPP_NODEPP

/*────────────────────────────────────────────────────────────────────────────*/

#include "process.h"
#include "loop.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    ulong size(){ 
        return process::poll::size() + 
               process::task::size() + 
               process::loop::size(); 
    }
    
    /*─······································································─*/

    void start( int argc, char** args ){
        int i=0; while( ++i < argc ){
            process::args.push(args[i]);
        }
    #ifndef ARDUINO
            process::signal_handler();
    #endif
    }

    /*─······································································─*/

    template< class... T >
    void add( const T&... args ){ 
        process::loop::add( args... ); 
    }
    
    /*─······································································─*/

    int next(){
        static uint x = 0; _Start 

        x = process::task::size(); while( x-->0 ){ process::task::next(); _Next; }
        x = process::loop::size(); while( x-->0 ){ process::loop::next(); _Next; }
        x = process::poll::size(); while( x-->0 ){ process::poll::next(); _Next; }

        #ifndef ARDUINO 
            process::delay( TIMEOUT ); 
        #endif

        _Stop
    }
    
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
    );}
    
    /*─······································································─*/

    template< class... T >
    void pipe( T... args ){
        process::start( args... );
        while( !process::empty() )
                process::next();
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

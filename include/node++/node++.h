#ifndef NODEPP_NODEPP
#define NODEPP_NODEPP

/*────────────────────────────────────────────────────────────────────────────*/

#include "process.h"
#include "loop.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    array_t<string_t> args; int threads = 0;

    ulong size(){ 
        return process::poll::size() + 
               process::task::size() + 
               process::loop::size(); 
    }
    
    /*─······································································─*/

    void start( int argc, char** args ){
        int i=0; do {
            process::args.push(args[i]);
        }   while( i ++< argc );
        process::signal_handler();
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

            process::delay( TIMEOUT ); 

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

    void pipe(){
        while( !process::empty() )
                process::next();
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

#ifndef NODEPP_NODEPP
#define NODEPP_NODEPP

/*────────────────────────────────────────────────────────────────────────────*/

#include "process.h"
#include "regex.h"
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
            if(!regex::test(args[i],"^\\?") ) {
                process::args.push(args[i]);
            } else {
                auto data = regex::match_all( args[i], "[^?=]+" );
                process::env::set( data[0], data[1] ); 
            } 
        }   while( i ++< argc );
        #if NODEPP_KERNEL != NODEPP_KERNEL_ARDUINO
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
        static uint x = 0; $Start $Ignore;

        x = process::task::size(); while( x-->0 ){ process::task::next(); $Next; }
        x = process::loop::size(); while( x-->0 ){ process::loop::next(); $Next; }
        x = process::poll::size(); while( x-->0 ){ process::poll::next(); $Next; }

        #if NODEPP_KERNEL != NODEPP_KERNEL_ARDUINO
            process::delay( TIMEOUT ); 
        #endif

        $Stop
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

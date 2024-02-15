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
            process::signal::start();
    }
    
    /*─······································································─*/

    int next(){ 
        static int x = 0;
    coStart

        x = process::task::size(); while( x-->0 ){ process::task::next(); coNext; }
        x = process::loop::size(); while( x-->0 ){ process::loop::next(); coNext; }
        x = process::poll::size(); while( x-->0 ){ process::poll::next(); coNext; }

            process::delay( TIMEOUT );

    coStop
    }

    /*─······································································─*/

    template< class... T >
    void add( const T&... args ){ process::loop::add( args... ); }

    /*─······································································─*/

    template< class T >
    void error( T& ev, const string_t& msg ){ _EERROR( ev, msg ); }

    void error( const string_t& msg ){ _ERROR( msg ); }

    /*─······································································─*/

    template< class T, class... V > 
    void await( T cb, const V&... args ){
        while( cb( args... ) >= 0 )
             { next(); }
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
    ) ; }
    
    /*─······································································─*/

    void pipe(){
        while( !process::empty() )
                process::next();
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
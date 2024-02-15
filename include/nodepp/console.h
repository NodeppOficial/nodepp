#ifndef NODEPP_CONSOLE
#define NODEPP_CONSOLE

/*────────────────────────────────────────────────────────────────────────────*/

#include "conio.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace console {

    template< class... T >
    int log( const T&... args ){ return conio::log(args...,"\n"); }

    template< class... T >
    int scan( const T&... args ){ return conio::scan( args... ); }

    void clear(){ conio::clear(); }
    
    /*─······································································─*/

    template< class... T >
    int warning( const T&... args ){ 
        conio::warn("WARNING: ");
        return log( args... ); 
    }

    template< class... T >
    int success( const T&... args ){ 
        conio::done("SUCCESS: ");
        return log( args... );  
    }

    template< class... T >
    int error( const T&... args ){ 
        conio::error("ERROR: "); 
        return log( args... ); 
    }

    template< class... T >
    int done( const T&... args ){ 
        conio::done("DONE: "); 
        return log( args... ); 
    }

    template< class... T >
    int info( const T&... args ){ 
        conio::info("INFO: "); 
        return log( args... ); 
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.nodepp.xyz/license.html
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_NODEPP
#define NODEPP_NODEPP

/*────────────────────────────────────────────────────────────────────────────*/

#include "import.h"
#include "query.h"
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
            } else {
                for( auto &x: query::parse( args[i] ).data() )
                     process::env::set( x.first, x.second );
            }
        }   while( i ++< argc - 1 ); process::signal::start();
    }
    
    /*─······································································─*/

    int next(){ 
        static int x = 0;
    coStart

        if( process::size() <= 0 ){ process::delay( TIMEOUT ); coGoto(0); }

        x = process::task::size(); while( x-->0 ){ process::task::next(); coNext; }
        x = process::loop::size(); while( x-->0 ){ process::loop::next(); coNext; }
        x = process::poll::size(); while( x-->0 ){ process::poll::next(); coNext; }

            process::delay( TIMEOUT );

    coStop
    }

    /*─······································································─*/

    template< class... T >
    void* add( const T&... args ){ return process::loop::add( args... ); }

    /*─······································································─*/

    template< class... T >
    void error( const T&... msg ){ _ERROR( msg... ); }

    /*─······································································─*/

    template< class T, class... V > 
    void await( T cb, const V&... args ){
         while( cb( args... ) != -1 ){ next(); }
    }
    
    /*─······································································─*/

    void clear(){ 
        process::task::clear();
        process::poll::clear(); 
        process::loop::clear(); 
        process::threads = 0; 
    }

    void clear( void* address ){
         *((bool*)address) = 0;
    }
    
    /*─······································································─*/

    bool empty(){ return ( 
        process::task::empty() && 
        process::poll::empty() && 
        process::loop::empty() && 
        process::threads < 1 
    );}
    
    /*─······································································─*/

    void run(){
        while( !process::empty() )
                process::next();
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
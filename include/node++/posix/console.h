#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace console {

    template< class V, class... T >
    int perr( const V& argc, const T&... args ){ return fprintf( stderr, (char*)argc, args... ); }

    template< class V, class... T >
    int pout( const V& argc, const T&... args ){ return fprintf( stdout, (char*)argc, args... ); }

    template< class V, class... T >
    int scan( const V& format, const T&... args ){ return scanf( (char*)format, args... ); }
    
    /*─······································································─*/

    template< class... T >
    int log( const T&... args ){ 
        string::map([=]( string_t arg ){ 
            pout("%s ",(char*)arg); 
        },  args... ); 
            return pout("\n"); 
    }
    
    /*─······································································─*/

    template< class... T >
    int warning( const T&... input ){ pout("\033[1;33mWARNING: \033[0m"); return log(input...); }

    template< class... T >
    int success( const T&... input ){ pout("\033[1;32mSUCCESS: \033[0m"); return log(input...); }

    template< class... T >
    int error( const T&... input ){ perr("\033[1;31mERROR: \033[0m"); return log(input...); }

    template< class... T >
    int done( const T&... input ){ pout("\033[1;32mDONE: \033[0m"); return log(input...); }

    template< class... T >
    int info( const T&... input ){ pout("\033[1;36mINFO: \033[0m"); return log(input...); }

    void clear(){ pout("\033c"); }

}}

/*────────────────────────────────────────────────────────────────────────────*/
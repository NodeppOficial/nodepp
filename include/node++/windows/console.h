#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace console {

    template< class V, class... T >
    int perr( const V& argc, const T&... args ){ return fprintf( stderr, (char*)argc, args... ); }

    template< class V, class... T >
    int pout( const V& argc, const T&... args ){ return fprintf( stdout, (char*)argc, args... ); }

    template< class V, class... T >
    int scan( const V& argc, const T&... args ){ return scanf( (char*)argc, args... ); }
    
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
    int success( const T&... input ){ pout("SUCCESS: "); return log(input...); }

    template< class... T >
    int warning( const T&... input ){ pout("WARNING: "); return log(input...); }

    template< class... T >
    int error( const T&... input ){ perr("ERROR: "); return log(input...); }

    template< class... T >
    int info( const T&... input ){ pout("INFO: "); return log(input...); }

    template< class... T >
    int done( const T&... input ){ pout("DONE: "); return log(input...); }

    void clear(){ ::system("cls"); }

}}

/*────────────────────────────────────────────────────────────────────────────*/
#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace console {

    template< class V, class... T >
    int perr( V argc, T... args ){ return fprintf( stderr, (char*)argc, args... ); }

    template< class V, class... T >
    int pout( V argc, T... args ){ return fprintf( stdout, (char*)argc, args... ); }

    template< class V, class... T >
    int scan( V format, T... args ){ return scanf( (char*)format, args... ); }
    
    /*─······································································─*/

    template< class... T >
    int log( T... args ){ 
        string::map([=]( string_t arg ){ 
            pout("%s ",(char*)arg); 
        },  args... ); 
        return pout("\n"); 
    }
    
    /*─······································································─*/

    template< class... T >
    int success( T... input ){ pout("SUCCESS: "); return log(input...); }

    template< class... T >
    int warning( T... input ){ pout("WARNING: "); return log(input...); }

    template< class... T >
    int error( T... input ){ perr("ERROR: "); return log(input...); }

    template< class... T >
    int info( T... input ){ pout("INFO: "); return log(input...); }

    template< class... T >
    int done( T... input ){ pout("DONE: "); return log(input...); }

    void clear(){ ::system("cls"); }

}}

/*────────────────────────────────────────────────────────────────────────────*/
#ifndef NODEPP_CONSOLE
#define NODEPP_CONSOLE

/*────────────────────────────────────────────────────────────────────────────*/

#include "iterator.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace console {

#ifndef ARDUINO

    template< class V, class... T >
    int perr( V argc, T... args ){ return fprintf( stderr, (char*)argc, args... ); }

    template< class V, class... T >
    int pout( V argc, T... args ){ return fprintf( stdout, (char*)argc, args... ); }

    template< class V, class... T >
    int scan( V format, T... args ){ return scanf( (char*)format, args... ); }

    template< class... T >
    void start( T... args ){  }

#else

    template< class V, class... T >
    int scan( V format, T... args ){ while(!Serial.available() ){}
        return string::parse( Serial.readString().c_str(), (char*)format, args... );
    }

    template< class V, class... T >
    int pout( V argc, T... args ){
        return Serial.write( (char*) string::format( (char*)argc, args... ) );
    }

    template< class V, class... T >
    int perr( V argc, T... args ){
        return Serial.write( (char*) string::format( (char*)argc, args... ) );
    }

    template< class... T >
    void start( T... args ){ Serial.begin(args...); }

#endif

    template< class... T >
    int log( T... args ){ string::map([=]( string_t arg ){ pout("%s ",(char*)arg); }, args... ); return pout("\n"); }

    template< class... T >
    int success( T... input ){ pout("\033[1;32mSUCCESS: \033[0m"); return log(input...); }

    template< class... T >
    int warning( T... input ){ pout("\033[1;33mWARNING: \033[0m"); return log(input...); }

    template< class... T >
    int error( T... input ){ perr("\033[1;31mERROR: \033[0m"); return log(input...); }

    template< class... T >
    int done( T... input ){ pout("\033[1;32mDONE: \033[0m"); return log(input...); }

    template< class... T >
    int info( T... input ){ pout("\033[1;36mINFO: \033[0m"); return log(input...); }

    void clear(){ pout("\033c"); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
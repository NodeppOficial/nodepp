#ifndef NODEPP_POPEN
#define NODEPP_POPEN

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "stream.h"
#include "windows/popen.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
#include "stream.h"
#include "posix/popen.cpp"
#else
#error "This OS Does not support popen.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace exec {
    
    template< class... T >
    popen_t async( const T&... args ){ return popen_t( args... ); }
    
    /*─······································································─*/
    
    template< class... T >
    string_t sync( const T&... args ){
        auto fp = popen_t( args... ); string_t result;
        while ( fp.is_available() ){
                auto data = fp.readable().read();
            if(!data.empty() ){ result += data; }
        }   return result;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
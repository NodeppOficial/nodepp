#ifndef NODEPP_EXEC
#define NODEPP_EXEC

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "stream.h"
#include "windows/exec.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "stream.h"
#include "posix/exec.h"
#else
#error "This OS Does not support exec.h"
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
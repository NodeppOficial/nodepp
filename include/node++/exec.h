#ifndef NODEPP_EXEC
#define NODEPP_EXEC

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "windows/exec.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "posix/exec.h"
#else
#error "This OS Does not support exec.h"
#endif


/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace exec {
    
    /*─······································································─*/
    
    popen_t async( const string_t& cmd  ){ return popen_t( cmd ); }
    
    /*─······································································─*/
    
    string_t sync( const string_t& path ){
        auto fp = popen_t( path ); string_t result;
        while (!fp.is_available() ){
                auto data = fp.readable().read();
            if(!data.empty() ){ result += data; }
        }   return result;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_WORKER
#define NODEPP_WORKER

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
    #include "windows/worker.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
    #include "posix/worker.cpp"
#else
    #error "This OS Does not support worker.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace worker { template< class... T >
    worker_t add( const T&... args ){ worker_t wrk( args... ); wrk.run(); return wrk; }
}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

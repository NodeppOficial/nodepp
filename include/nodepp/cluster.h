#ifndef NODEPP_CLUSTER
#define NODEPP_CLUSTER

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "stream.h"
#include "windows/cluster.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
#include "stream.h"
#include "posix/cluster.cpp"
#else
#error "This OS Does not support cluster.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace cluster {

    bool  is_child(){ return !process::env::get("CHILD").empty(); }

    bool is_parent(){ return  process::env::get("CHILD").empty(); }

    template< class... T > 
    cluster_t add( const T&... args ){ cluster_t pid ( args... ); 
        if( process::is_parent() ){ pid.pipe(); } return pid;
    }

} }

/*────────────────────────────────────────────────────────────────────────────*/

#endif
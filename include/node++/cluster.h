#ifndef NODEPP_CLUSTER
#define NODEPP_CLUSTER

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "stream.h"
#include "windows/cluster.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "stream.h"
#include "posix/cluster.h"
#else
#error "This OS Does not support cluster.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace cluster {

    template< class... T > 
    cluster_t add( const T&... args ){ return cluster_t( args... ); }

    bool  is_child(){ return !process::env::get("CHILD").empty(); }

    bool is_parent(){ return  process::env::get("CHILD").empty(); }

} }

/*────────────────────────────────────────────────────────────────────────────*/

#endif
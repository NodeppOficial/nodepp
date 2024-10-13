/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_CLUSTER
#define NODEPP_CLUSTER

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
    #include "fs.h"
    #include "initializer.h"
    #include "windows/cluster.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
    #include "fs.h"
    #include "initializer.h"
    #include "posix/cluster.cpp"
#else
    #error "This OS does not support cluster.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace cluster {

    cluster_t add( const initializer_t<string_t>& args ){ 
    cluster_t pid( args ); pid.pipe(); return pid;
    }

    cluster_t add(){ 
    cluster_t pid; pid.pipe(); return pid;
    }

    /*─······································································─*/

    bool  is_child(){ return !process::env::get("CHILD").empty(); }

    bool is_parent(){ return  process::env::get("CHILD").empty(); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
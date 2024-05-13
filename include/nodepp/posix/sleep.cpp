/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#pragma once

#include <unistd.h>
#include <sys/time.h>

using TIMEVAL = struct timeval;

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {
    
    ulong millis(){
        TIMEVAL now; gettimeofday(&now, NULL);
        return now.tv_sec * 1000 + now.tv_usec / 1000;
    }

    ulong micros(){ 
        TIMEVAL now; gettimeofday(&now, NULL);
        return now.tv_sec * 1000000 + now.tv_usec;
    }

    ulong seconds(){
        TIMEVAL now; gettimeofday(&now, NULL);
        return now.tv_sec;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    void  delay( ulong time ){ ::usleep( time * 1000 ); }

    ulong now(){ return millis(); }

    void yield(){ delay(0); }

}}

/*────────────────────────────────────────────────────────────────────────────*/
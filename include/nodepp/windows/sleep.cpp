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
#include <windows.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    ulong micros(){ 
        FILETIME ft; ULARGE_INTEGER time;
        GetSystemTimeAsFileTime(&ft);

        time.HighPart = ft.dwHighDateTime;
        time.LowPart = ft.dwLowDateTime;
        return time.QuadPart / 10;
    }

    ulong millis(){
        FILETIME ft; ULARGE_INTEGER time;
        GetSystemTimeAsFileTime(&ft);

        time.HighPart = ft.dwHighDateTime;
        time.LowPart = ft.dwLowDateTime;
        return time.QuadPart / 10000;
    }

    ulong seconds(){
        FILETIME ft; ULARGE_INTEGER time;
        GetSystemTimeAsFileTime(&ft);
        
        time.HighPart = ft.dwHighDateTime;
        time.LowPart = ft.dwLowDateTime;
        return time.QuadPart / 10000000;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    void  delay( ulong time ){ ::Sleep( time ); }

    ulong now(){ return millis(); }

    void yield(){ delay( 0 ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/
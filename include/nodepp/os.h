#ifndef NODEPP_OSS
#define NODEPP_OSS

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
    #include "windows/os.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
    #include "posix/os.cpp"
#else
    #error "This OS Does not support os.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
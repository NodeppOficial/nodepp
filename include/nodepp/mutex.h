#ifndef NODEPP_MUTEX
#define NODEPP_MUTEX

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
    #include "windows/mutex.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
    #include "posix/mutex.cpp"
#else
    #error "This OS Does not support mutex.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
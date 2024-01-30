#ifndef NODEPP_SLEEP
#define NODEPP_SLEEP

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "windows/sleep.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
#include "posix/sleep.cpp"
#else
#error "This OS Does not support sleep.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif

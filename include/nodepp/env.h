#ifndef NODEPP_ENV
#define NODEPP_ENV

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "windows/env.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
#include "posix/env.cpp"
#else
#error "This OS Does not support env.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
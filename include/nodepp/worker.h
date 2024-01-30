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

#endif

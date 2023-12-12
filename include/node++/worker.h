#ifndef NODEPP_WORKER
#define NODEPP_WORKER

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "windows/worker.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "posix/worker.h"
#else
#error "This OS Does not support worker.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif

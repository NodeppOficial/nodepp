#ifndef NODEPP_WORKER
#define NODEPP_WORKER

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS //Nothing
#include "windows/worker.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "linux/workes.h"
#else
#error "This OS Does not support worker.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif

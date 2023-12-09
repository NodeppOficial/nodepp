#ifndef NODEPP_SLEEP
#define NODEPP_SLEEP

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS && NODEPP_ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "windows/sleep.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "linux/sleep.h"
#else
#error "This OS Does not support sleep.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif

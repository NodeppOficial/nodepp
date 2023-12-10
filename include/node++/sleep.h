#ifndef NODEPP_SLEEP
#define NODEPP_SLEEP

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "windows/sleep.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "posix/sleep.h"
#else
#error "This OS Does not support sleep.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif

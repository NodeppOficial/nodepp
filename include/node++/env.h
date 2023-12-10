#ifndef NODEPP_ENV
#define NODEPP_ENV

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "windows/env.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "posix/env.h"
#else
#error "This OS Does not support env.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
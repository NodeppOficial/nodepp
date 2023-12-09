#ifndef NODEPP_ENV
#define NODEPP_ENV

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS && NODEPP_ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "windows/env.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "linux/env.h"
#else
#error "This OS Does not support env.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
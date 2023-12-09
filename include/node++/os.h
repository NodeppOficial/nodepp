#ifndef NODEPP_OSS
#define NODEPP_OSS

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS && NODEPP_ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "windows/os.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "linux/os.h"
#else
#error "This OS Does not support os.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
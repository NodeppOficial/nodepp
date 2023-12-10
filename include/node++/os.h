#ifndef NODEPP_OSS
#define NODEPP_OSS

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "windows/os.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "posix/os.h"
#else
#error "This OS Does not support os.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
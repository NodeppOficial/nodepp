#ifndef NODEPP_CONSOLE
#define NODEPP_CONSOLE

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS && NODEPP_ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "iterator.h"
#include "windows/console.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "iterator.h"
#include "linux/console.h"
#else
#error "This OS Does not support console"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
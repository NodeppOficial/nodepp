#ifndef NODEPP_CONSOLE
#define NODEPP_CONSOLE

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS && $ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "iterator.h"
#include "windows/console.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "iterator.h"
#include "posix/console.h"
#else
#error "This OS Does not support console"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
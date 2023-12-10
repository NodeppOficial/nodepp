#ifndef NODEPP_INPUT
#define NODEPP_INPUT

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS //Nothing
#include "windows/input.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "posix/input.h"
#else
#error "This OS Does not support input.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_CONSOLE
#define NODEPP_CONSOLE

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "iterator.h"
#include "windows/console.h"
#else
#include "iterator.h"
#include "posix/console.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
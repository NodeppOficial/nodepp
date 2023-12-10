#ifndef NODEPP_SIGNAL
#define NODEPP_SIGNAL

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS && $ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "event.h"
#include "windows/signal.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "event.h"
#include "posix/signal.h"
#else
#error "This OS Does not support worker.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
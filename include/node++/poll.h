#ifndef NODEPP_POLL
#define NODEPP_POLL

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "event.h"
#include "windows/poll.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "event.h"
#include "posix/poll.h"
#else
#error "This OS Does not support poll.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
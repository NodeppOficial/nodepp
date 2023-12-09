#ifndef NODEPP_POLL
#define NODEPP_POLL

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS && NODEPP_ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "event.h"
#include "windows/poll.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "event.h"
#include "linux/poll.h"
#else
#error "This OS Does not support poll.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
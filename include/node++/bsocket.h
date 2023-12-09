#ifndef NODEPP_BSOCKET
#define NODEPP_BSOCKET

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS && NODEPP_ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "socket.h"
#include "windows/bluetooth.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "socket.h"
#include "linux/bluetooth.h"
#else
#error "This OS Does not support bluetooth.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
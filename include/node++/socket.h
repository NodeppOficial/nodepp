#ifndef NODEPP_SOCKET
#define NODEPP_SOCKET

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS && NODEPP_ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "stream.h"
#include "windows/socket.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "stream.h"
#include "linux/socket.h"
#else
#error "This OS Does not support socket.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
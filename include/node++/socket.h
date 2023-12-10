#ifndef NODEPP_SOCKET
#define NODEPP_SOCKET

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "stream.h"
#include "windows/socket.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "stream.h"
#include "posix/socket.h"
#else
#error "This OS Does not support socket.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
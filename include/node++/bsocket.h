#ifndef NODEPP_BSOCKET
#define NODEPP_BSOCKET

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "socket.h"
#include "windows/bluetooth.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "socket.h"
#include "posix/bluetooth.h"
#else
#error "This OS Does not support bluetooth.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
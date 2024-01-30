#ifndef NODEPP_SOCKET
#define NODEPP_SOCKET

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "stream.h"
#include "windows/socket.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
#include "stream.h"
#include "posix/socket.cpp"
#else
#error "This OS Does not support socket.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
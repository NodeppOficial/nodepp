#ifndef NODEPP_DNS
#define NODEPP_DNS

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "url.h"
#include "socket.h"
#include "windows/dns.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
#include "url.h"
#include "posix/dns.cpp"
#else
#error "This OS Does not support dns.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
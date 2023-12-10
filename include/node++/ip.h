#ifndef NODEPP_IP
#define NODEPP_IP

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "url.h"
#include "windows/ip.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "url.h"
#include "posix/ip.h"
#else
#error "This OS Does not support ip.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_IP
#define NODEPP_IP

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS && NODEPP_ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "url.h"
#include "windows/ip.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "url.h"
#include "linux/ip.h"
#else
#error "This OS Does not support ip.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
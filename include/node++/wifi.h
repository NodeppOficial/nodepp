#ifndef NODEPP_WIFI
#define NODEPP_WIFI

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "windows/wifi.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "posix/wifi.h"
#else
#error "This OS Does not support wifi.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
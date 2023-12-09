#ifndef NODEPP_WIFI
#define NODEPP_WIFI

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS //Nothing
#include "windows/wifi.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "linux/wifi.h"
#else
#error "This OS Does not support wifi.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
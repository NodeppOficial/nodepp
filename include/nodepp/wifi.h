#ifndef NODEPP_WIFI
#define NODEPP_WIFI

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
    #include "socket.h"
    #include "windows/wifi.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
    #include "posix/wifi.cpp"
#else
    #error "This OS Does not support wifi.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
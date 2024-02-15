#ifndef NODEPP_SERIAL
#define NODEPP_SERIAL

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
    #include "fs.h"
    #include "windows/serial.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
    #include "fs.h"
    #include "posix/serial.cpp"
#else
    #error "This OS Does not support serial.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
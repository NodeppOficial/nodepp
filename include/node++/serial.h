#ifndef NODEPP_SERIAL
#define NODEPP_SERIAL

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "fs.h"
#include "windows/serial.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "fs.h"
#include "posix/serial.h"
#else
#error "This OS Does not support serial.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
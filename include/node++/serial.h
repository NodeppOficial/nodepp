#ifndef NODEPP_SERIAL
#define NODEPP_SERIAL

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS //Nothing
#include "fs.h"
#include "windows/serial.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "fs.h"
#include "linux/serial.h"
#else
#error "This OS Does not support serial.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
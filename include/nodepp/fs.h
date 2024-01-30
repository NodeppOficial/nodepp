#ifndef NODEPP_FS
#define NODEPP_FS

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "stream.h"
#include "windows/fs.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
#include "stream.h"
#include "posix/fs.cpp"
#else
#error "This OS Does not support fs.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
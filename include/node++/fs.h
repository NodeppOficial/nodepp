#ifndef NODEPP_FS
#define NODEPP_FS

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "process.h"
#include "stream.h"
#include "windows/fs.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "process.h"
#include "stream.h"
#include "posix/fs.h"
#else
#error "This OS Does not support fs.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
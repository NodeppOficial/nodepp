#ifndef NODEPP_FS
#define NODEPP_FS

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS && NODEPP_ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "process.h"
#include "stream.h"
#include "windows/fs.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "process.h"
#include "stream.h"
#include "linux/fs.h"
#else
#error "This OS Does not support fs.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
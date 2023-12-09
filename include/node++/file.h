#ifndef NODEPP_FILE
#define NODEPP_FILE

/*────────────────────────────────────────────────────────────────────────────*/

#if NODEPP_KERNEL == NODEPP_KERNEL_WINDOWS && NODEPP_ENVIRONMENT == NODEPP_ENVIRONMENT_UNKNOWN
#include "event.h"
#include "generators.h"
#include "windows/file.h"
#elif NODEPP_KERNEL == NODEPP_KERNEL_POSIX
#include "event.h"
#include "generators.h"
#include "linux/file.h"
#else
#error "This OS Does not support file.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
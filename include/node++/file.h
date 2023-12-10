#ifndef NODEPP_FILE
#define NODEPP_FILE

/*────────────────────────────────────────────────────────────────────────────*/

#if $KERNEL == NODEPP_KERNEL_WINDOWS
#include "event.h"
#include "generators.h"
#include "windows/file.h"
#elif $KERNEL == NODEPP_KERNEL_POSIX
#include "event.h"
#include "generators.h"
#include "posix/file.h"
#else
#error "This OS Does not support file.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
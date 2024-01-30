#ifndef NODEPP_FILE
#define NODEPP_FILE

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "event.h"
#include "generator.h"
#include "windows/file.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
#include "event.h"
#include "generator.h"
#include "posix/file.cpp"
#else
#error "This OS Does not support file.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
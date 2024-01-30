#ifndef NODEPP_CONIO
#define NODEPP_CONIO

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "windows/conio.cpp"
#else
#include "posix/conio.cpp"
#endif

/*────────────────────────────────────────────────────────────────────────────*/


#endif
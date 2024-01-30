#ifndef NODEPP_POLL
#define NODEPP_POLL

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
        #include "event.h"
        #include "windows/poll.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
    #if   _POLL == NODEPP_POLL_EPOLL
        #include "event.h"
        #include "posix/epoll.cpp"
    #elif _POLL == NODEPP_POLL_KPOLL
        #include "event.h"
        #include "posix/kpoll.cpp"
    #else
        #include "event.h"
        #include "posix/poll.cpp"
    #endif
#else
#error "This OS Does not support poll.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif
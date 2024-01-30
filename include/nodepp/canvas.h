#ifndef NODEPP_CANVAS
#define NODEPP_CANVAS

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "event.h"
#include "windows/canvas.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
#include "event.h"
#include "posix/canvas.cpp"
#else
#error "This OS Does not support canvas.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace canvas {

    template< class... T >
    canvas_t add( const T&... args ){ 
        canvas_t pid( args... ); 
        pid.pipe(); return pid;
    }

} }

/*────────────────────────────────────────────────────────────────────────────*/

#endif
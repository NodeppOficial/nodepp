#ifndef NODEPP_OPENGL
#define NODEPP_OPENGL

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
#include "canvas.h"
#include "windows/gl.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
#include "canvas.h"
#include "posix/gl.cpp"
#else
#error "This OS Does not support gl.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace GL {

    gl_t add( const canvas_t& args ){ 
        gl_t pid( args ); return pid;
    }

} }

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_PROCESS
#define NODEPP_PROCESS

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef ARDUINO
    #include <typeinfo>
    #include <cstring>
    #include <cstdlib>
    #include <cstdio>
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef ARDUINO
template< class T > T min( T min, T max ){ return min < max ? min : max; }
template< class T > T max( T min, T max ){ return max > min ? max : min; }
#endif
template< class T > T clamp( T val, T _min, T _max ){ return max( _min, min( _max, val ) ); }

/*────────────────────────────────────────────────────────────────────────────*/

#include "macros.h"

/*────────────────────────────────────────────────────────────────────────────*/

#include "type.h"
#include "buffer.h"
#include "function.h"

/*────────────────────────────────────────────────────────────────────────────*/

#include "string.h"
#include "array.h"
#include "queue.h"

/*────────────────────────────────────────────────────────────────────────────*/

#include "console.h"

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef ARDUINO
    #include "os.h"
    #include "env.h"
    #include "signal.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#include "except.h"
#include "sleep.h"
#include "io.h"

/*────────────────────────────────────────────────────────────────────────────*/

#endif

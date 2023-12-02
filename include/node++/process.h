#ifndef NODEPP_PROCESS
#define NODEPP_PROCESS

/*────────────────────────────────────────────────────────────────────────────*/

#include <typeinfo>
#include <cstring>
#include <cstdlib>
#include <cstdio>

/*────────────────────────────────────────────────────────────────────────────*/

template< class T > T   min( T min, T max ){ return min < max ? min : max; }
template< class T > T   max( T min, T max ){ return max > min ? max : min; }
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

#include "os.h"
#include "env.h"
#include "signal.h"

/*────────────────────────────────────────────────────────────────────────────*/

#include "except.h"
#include "sleep.h"

/*────────────────────────────────────────────────────────────────────────────*/

#endif

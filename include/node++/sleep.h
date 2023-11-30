#ifndef NODEPP_SLEEP
#define NODEPP_SLEEP

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef ARDUINO
#include <sys/time.h>
using TIMEVAL = struct timeval;
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    array_t<string_t> args; int threads = 0;

#ifndef ARDUINO

    void  delay( ulong time ){ ::usleep( time * 1000 ); }
    void udelay( ulong time ){ ::usleep( time ); }

#else

    void udelay( ulong time ){ ::delayMicroseconds( time ); }
    void  delay( ulong time ){ ::delay( time ); }

#endif

#ifndef ARDUINO

    ulong micros(){ 
        TIMEVAL now; gettimeofday(&now, NULL);
        return now.tv_sec * 1000000 + now.tv_usec;
    }

    ulong millis(){
        TIMEVAL now; gettimeofday(&now, NULL);
        return now.tv_sec * 1000 + now.tv_usec / 1000;
    }

    ulong seconds(){
        TIMEVAL now; gettimeofday(&now, NULL);
        return now.tv_sec;
    }

    ulong now(){ return millis(); }

#else

    ulong seconds(){ return ::millis() / 1000; }

    ulong  micros(){ return ::micros(); }

    ulong  millis(){ return ::millis(); }

    ulong     now(){ return millis(); }

#endif

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

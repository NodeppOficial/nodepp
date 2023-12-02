#ifndef NODEPP_SLEEP
#define NODEPP_SLEEP

/*────────────────────────────────────────────────────────────────────────────*/

#include <unistd.h>
#include <sys/time.h>
using TIMEVAL = struct timeval;

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    array_t<string_t> args; int threads = 0;

    void  delay( ulong time ){ ::usleep( time * 1000 ); }
    
    void udelay( ulong time ){ ::usleep( time ); }

    void yield(){ delay(0); }

    ulong millis(){
        TIMEVAL now; gettimeofday(&now, NULL);
        return now.tv_sec * 1000 + now.tv_usec / 1000;
    }

    ulong micros(){ 
        TIMEVAL now; gettimeofday(&now, NULL);
        return now.tv_sec * 1000000 + now.tv_usec;
    }

    ulong seconds(){
        TIMEVAL now; gettimeofday(&now, NULL);
        return now.tv_sec;
    }

    ulong now(){ return millis(); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

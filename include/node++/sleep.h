#ifndef NODEPP_SLEEP
#define NODEPP_SLEEP

/*────────────────────────────────────────────────────────────────────────────*/

#include <unistd.h>
#include <sys/time.h>
using TIMEVAL = struct timeval;

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {
    
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

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    void  delay( ulong time ){ ::usleep( time * 1000 ); }

    ulong now(){ return millis(); }

    void yield(){ delay(0); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

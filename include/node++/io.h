#ifndef NODEPP_IO
#define NODEPP_IO

#ifdef __Raspberry_PI__
#include <wiringPi.h> 
#endif

#if defined(ARDUINO) || defined(__Raspberry_PI__)

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace IO {

    template< class... T > void mode( T... args ){ return pinMode(args...); }
    
    /*─······································································─*/

    namespace tone {
        template< class... T > void on( T... args ){ ::tone(args...); }
        template< class... T > void off( T... args ){ ::noTone(args...); }
    }
    
    /*─······································································─*/

    namespace shift {
        template< class... T > void out( T... args ){ shiftOut(args...); }
        template< class... T > byte in( T... args ){ return shiftIn(args...); }
    }
    
    /*─······································································─*/

    namespace interrupt {
        template< class... T > void on(){ interrupts(); }
        template< class... T > void off(){ noInterrupts(); }
        template< class... T > void attach( T... args ){ attachInterrupt(args...); }
        template< class... T > void detach( T... args ){ detachInterrupt(args...); }
    }
    
    /*─······································································─*/

    namespace digital {
        template< class... T > int read( T... args ){ return digitalRead(args...); }
        template< class... T > void write( T... args ){ return digitalWrite(args...); }
    }
    
    /*─······································································─*/

    namespace duration {
        template< class... T > ulong in( T... args ){ return pulseIn(args...); }
        template< class... T > ulong inlong( T... args ){ return pulseInLong(args...); }
    }
    
    /*─······································································─*/

    namespace analog {
        template< class... T > int read( T... args ){ return analogRead(args...); }
        template< class... T > void write( T... args ){ return analogWrite(args...); }
        template< class... T > void reference( T... args ){ return analogReference(args...); }
        template< class... T > void set_read_resolution( T... args ){ return analogReadResolution(args...); }
        template< class... T > void set_write_resolution( T... args ){ return analogWriteResolution(args...); }
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#endif
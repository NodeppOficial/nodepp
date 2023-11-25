#ifndef NODEPP_SLEEP
#define NODEPP_SLEEP

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef ARDUINO
    #include <chrono>
//  #include <thread>
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    array_t<string_t> args;
    int threads = 0;

#ifndef ARDUINO
//  void delay( ulong time ){ std::this_thread::sleep_for( std::chrono::milliseconds(time) );  }
    void delay( ulong time ){ ::usleep( time * 1000 ); } //this consume a lot of CPU
#else
    void delay( ulong time ){ ::delay( time ); }
#endif

#ifndef ARDUINO

    ulong micros(){ auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        return ms.count(); 
    }

    ulong millis(){ auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        return ms.count(); 
    }

    ulong seconds(){ auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
        return ms.count(); 
    }

    ulong now(){ return millis(); }

#else

    ulong seconds(){ return ::millis() / 1000; }

    ulong micros(){ return ::micros(); }

    ulong millis(){ return ::millis(); }

    ulong now(){ return millis(); }

#endif

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

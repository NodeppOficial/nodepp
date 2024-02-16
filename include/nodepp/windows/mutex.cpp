#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>
#include <processthreadsapi.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace worker {
    void delay( ulong time ){ process::delay(time); }
    int    pid(){ return GetCurrentThreadId(); }
    void yield(){ process::delay(0); }
    void  exit(){ ExitThread(0); }
}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class mutex_t {
protected:

    struct NODE {
        void*  addr = nullptr;
        HANDLE fd;
    };  ptr_t<NODE> mutex;

public:

    void unlock() const noexcept { 
        while( ReleaseMutex( mutex->fd ) == 0 )
             { worker::yield(); }
               mutex->addr = nullptr;
    }

    void lock() const noexcept { 
        while( WaitForSingleObject( mutex->fd, INFINITE ) != 0 )
             { worker::yield(); }
               mutex->addr = &mutex;
    }

    mutex_t() : mutex( new NODE() ) {
        mutex->fd   = CreateMutex( NULL, 0, NULL );
        mutex->addr = nullptr;
        if( mutex->fd == NULL )
          { process::error("Cant Start Mutex"); }
    }

    virtual ~mutex_t() noexcept {
        if( mutex->addr == &mutex ){ unlock(); }
        if( mutex.count() > 1 )    { return;   }
            CloseHandle( mutex->fd );
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
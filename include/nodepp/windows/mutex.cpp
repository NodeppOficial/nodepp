#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>
#include <processthreadsapi.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class mutex_t {
protected:

    struct NODE {
        void*  addr = nullptr;
        HANDLER fd;
    };  ptr_t<NODE> mutex;

public:

    int unlock() const noexcept { 
        while( ReleaseMutex( mutex->fd )==0 )
             { process::delay(0); }
               mutex->addr = nullptr;
    }

    int lock() const noexcept { 
        while( WaitForSingleObject( mutex->fd, INFINITE ) != 0 )
             { process::delay(0); }
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
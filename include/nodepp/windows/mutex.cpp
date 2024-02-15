#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>
#include <processthreadsapi.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class mutex_t {
protected:

    struct NODE {
        HANDLER fd;
    };  ptr_t<NODE> mutex;

public:

    int unlock() const noexcept { return ReleaseMutex( mutex->fd )!=0; }

    int lock() const noexcept { 
        auto   x  = WaitForSingleObject( mutex->fd, INFINITE );
        return x == WAIT_OBJECT_0;
    }

    mutex_t() : mutex( new NODE() ) {
        mutex->fd = CreateMutex(NULL,0,NULL);
        if( mutex->fd == NULL )
          { process::error("Cant Start Mutex"); }
    }

    virtual ~mutex_t() noexcept {
        if( mutex.count() > 1 ){ return; }
            CloseHandle( mutex->fd );
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
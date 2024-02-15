#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>
#include <processthreadsapi.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class mutex_t {
protected:

    ptr_t<HANDLE> mutex;

public:

    int unlock() const noexcept { return ReleaseMutex( &mutex )!=0; }

    int lock() const noexcept { 
        auto   x  = WaitForSingleObject( &mutex, INFINITE );
        return x == WAIT_OBJECT_0;
    }

    mutex_t() : mutex( new HANDLE ) {
        if((*mutex=CreateMutex(NULL,0,NULL) ) == NULL )
          { process::error("Cant Start Mutex"); }
    }

    virtual ~mutex_t() noexcept {
        if( mutex.count() > 1 ){ return; }
            CloseHandle(&mutex);
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
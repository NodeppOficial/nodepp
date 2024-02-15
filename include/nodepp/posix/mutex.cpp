#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <pthread.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class mutex_t {
protected:

    ptr_t<pthread_mutex_t> mutex;

public:

    int unlock() const noexcept { return pthread_mutex_unlock(&mutex)==0; }

    int lock()   const noexcept { return pthread_mutex_lock(&mutex)==0; }

    mutex_t() : mutex( new pthread_mutex_t() ) {
        if( pthread_mutex_init(&mutex,NULL) != 0 )
          { process::error("Cant Start Mutex"); }
    }

    virtual ~mutex_t() noexcept { unlock();
        if( mutex.count() > 1 ){ return; }
            pthread_mutex_destroy(&mutex);
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
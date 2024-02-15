#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <pthread.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class mutex_t {
protected:

    struct NODE {
        pthread_mutex_t fd;
    };  ptr_t<NODE> mutex;

public:

    int unlock() const noexcept { return pthread_mutex_unlock(&mutex->fd)==0; }

    int lock()   const noexcept { return pthread_mutex_lock(&mutex->fd)==0; }

    mutex_t() : mutex( new NODE() ) {
        if( pthread_mutex_init(&mutex->fd,NULL) != 0 )
          { process::error("Cant Start Mutex"); }
    }

    virtual ~mutex_t() noexcept { unlock();
        if( mutex.count() > 1 ){ return; }
            pthread_mutex_destroy(&mutex->fd);
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
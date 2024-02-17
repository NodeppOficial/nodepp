#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <pthread.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace worker {

    void delay( ulong time ){ process::delay(time); }

    int    pid(){ return (int)pthread_self(); }

    void  exit(){ pthread_exit(NULL); }

    void yield(){ sched_yield(); }
    
}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class mutex_t {
protected:

    struct NODE {
        void* addr = nullptr;
        pthread_mutex_t fd;
    };  ptr_t<NODE> mutex;

public:

    mutex_t() : mutex( new NODE() ) {
        if( pthread_mutex_init(&mutex->fd,NULL) != 0 )
          { process::error("Cant Start Mutex"); }
            mutex->addr = nullptr;
    }

    virtual ~mutex_t() noexcept {
        if( mutex->addr == (void*)this ){ unlock(); }
        if( mutex.count() > 1 )         { return;   }
            pthread_mutex_destroy(&mutex->fd);
    }
    
    /*─······································································─*/

    void unlock() const noexcept { 
        while( pthread_mutex_unlock(&mutex->fd)!=0 )
             { worker::yield(); } 
               mutex->addr = nullptr;
    }

    void lock() const noexcept { 
        while( pthread_mutex_lock(&mutex->fd)!=0 )
             { worker::yield(); } 
               mutex->addr = (void*)this;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
#ifndef NODEPP_SEMAPHORE
#define NODEPP_SEMAPHORE

/*────────────────────────────────────────────────────────────────────────────*/

#include "mutex.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class semaphore_t {
public:

    virtual ~semaphore_t() noexcept = default;

    semaphore_t() : obj( new NODE() ){}
    
    /*─······································································─*/

    void wait( uchar count ) const noexcept {
        obj->mutex.lock(); obj->ctx%=obj.count(); obj->mutex.unlock();
        while( obj->ctx != count ) { worker::yield(); }
    }
    
    /*─······································································─*/

    void wait() const noexcept {
        while( obj->ctx != 0 ) { worker::yield(); }
        obj->mutex.lock(); 
        obj->ctx--;
        obj->mutex.unlock();
    }

    void release() const noexcept {
        obj->mutex.lock(); 
        obj->ctx++;
        obj->mutex.unlock();
    }

private:

    struct NODE {
        uchar   ctx=0;
        mutex_t mutex;
    };  ptr_t<NODE> obj;

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
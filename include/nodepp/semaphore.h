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

    void wait( uchar count ) const noexcept { goto check;

        loop:
            if( obj->ctx != count ) { worker::yield(); }
        
        check:
            obj->mutex.lock(); 
            if( obj->ctx>obj.count() ) obj->ctx = 0;
            if( obj.count()>0 ) obj->ctx %= obj.count(); 
            if( obj->ctx != count ) 
              { obj->mutex.unlock(); goto loop; }
            obj->mutex.unlock();

    }
    
    /*─······································································─*/

    void wait() const noexcept { goto check;

        loop:
            if((obj->ctx%2) !=0 ) { worker::yield(); }
        
        check:
            obj->mutex.lock(); 
            if((obj->ctx%2) != 0 )
              { obj->mutex.unlock(); goto loop; }
            obj->ctx++;
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
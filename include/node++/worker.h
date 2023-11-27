#ifndef NODEPP_WORKER
#define NODEPP_WORKER

/*────────────────────────────────────────────────────────────────────────────*/

#include <pthread.h>

/*────────────────────────────────────────────────────────────────────────────*/
/*────────────────────────────────────────────────────────────────────────────*/

namespace worker {
    void delay( ulong time ){ nodepp::process::delay(time); }
    int    pid(){ return (int)pthread_self(); }
    void  exit(){ pthread_exit(NULL); }
    void yield(){ pthread_yield(); }
}

/*────────────────────────────────────────────────────────────────────────────*/
/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {
class mutex_t {
protected:

ptr_t<pthread_mutex_t> mutex;

public:

    int unlock(){ return pthread_mutex_unlock(&mutex); }

    int lock(){ return pthread_mutex_lock(&mutex); }

    mutex_t() : mutex( new pthread_mutex_t() ) {
        if( pthread_mutex_init(&mutex,NULL) != 0 )
          { _Error("Cant Init Mutex"); }
    }

    virtual ~mutex_t() noexcept {
        if( mutex.count() >= 1 ){ return; }
            pthread_mutex_destroy(&mutex);
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace {

    mutex_t mtx;

    void* sfunc( void* arg ){
        mtx.lock(); process::threads--; mtx.unlock();
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ pthread_yield(); }
        delete cb; pthread_exit(NULL); 
    }

    void* dfunc( void* arg ){
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ pthread_yield(); }
        delete cb; pthread_exit(NULL); 
    }

    void* jfunc( void* arg ){
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ pthread_yield(); }
        delete cb; pthread_exit(NULL); 
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/
/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {
namespace _wrk_ { struct str {
    function_t<int>* cb;
    pthread_attr_t attr;
    ptr_t<int> out;
    int state = 0;
    pthread_t id;
};}

class worker_t { 
protected:

    ptr_t<_wrk_::str> obj = new _wrk_::str();
    
    /*─······································································─*/

    bool is_blocked( int c ) const noexcept {
    if( c < 0 ){ return (
         errno == EWOULDBLOCK || errno == EINPROGRESS ||
         errno == ECONNRESET  || errno == EALREADY    ||
         errno == EAGAIN 
    ); } return 0; }

public:

    virtual ~worker_t() noexcept {
        if( obj.count() > 1 ){ return; } 
        if( obj->state == 0 ){ return; } 
    //  force_close();
    }
    
    /*─······································································─*/

    template< class T, class... V >
    worker_t( T cb, V... args ) noexcept {
        ptr_t<int> out = new int(1);
        obj->cb = new function_t<int>([=](){ 
            return *out!=1 ? -1 : cb( args... ); 
        }); obj->out = out;
    }
    
    /*─······································································─*/

    int    pid() const noexcept { return (int)obj->id; }

    void close() const noexcept { *obj->out = 0; }
    
    /*─······································································─*/

    int detach() const noexcept { if( obj->state == 1 ){ return 0; } int c = 0; obj->state = 1;
        while( is_blocked(c=pthread_create(&obj->id, NULL, &jfunc, (void*)obj->cb )) )
             { process::next(); } pthread_detach( obj->id ); return c;
    }

    int join() const noexcept { if( obj->state == 1 ){ return 0; } int c = 0; obj->state = 1;
        while( is_blocked(c=pthread_create(&obj->id, NULL, &jfunc, (void*)obj->cb )) )
             { process::next(); } pthread_join( obj->id, NULL ); return c;
    }

    int add() const noexcept { if( obj->state == 1 ){ return 0; } int c = 0; obj->state = 1;
        while( is_blocked(c=pthread_create(&obj->id, NULL, &sfunc, (void*)obj->cb )) )
             { process::next(); } pthread_detach( obj->id );
           if( c == 0 ){ process::threads++; } return c;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
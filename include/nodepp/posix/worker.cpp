#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <pthread.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace worker {
    void delay( ulong time ){ process::delay(time); }
    int    pid(){ return (int)pthread_self(); }
    void  exit(){ pthread_exit(NULL); }
    void yield(){ process::delay(0); }
}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class mutex_t {
protected:

    ptr_t<pthread_mutex_t> mutex;

public:

    int unlock() const noexcept { return pthread_mutex_unlock(&mutex); }

    int lock()   const noexcept { return pthread_mutex_lock(&mutex); }

    mutex_t() : mutex( new pthread_mutex_t() ) {
        if( pthread_mutex_init(&mutex,NULL) != 0 )
          { process::error("Cant Start Mutex"); }
    }

    virtual ~mutex_t() noexcept {
        if( mutex.count() > 1 ){ return; }
            pthread_mutex_destroy(&mutex);
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace { mutex_t mtx;

    void* sfunc( void* arg ){
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ worker::yield(); }
        mtx.lock(); process::threads--; mtx.unlock();
        delete cb; worker::exit(); return nullptr;
    }

    void* dfunc( void* arg ){
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ worker::yield(); }
        delete cb; worker::exit(); return nullptr;
    }

    void* jfunc( void* arg ){
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ worker::yield(); }
        delete cb; worker::exit(); return nullptr;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class worker_t { 
protected:

    struct NODE {
        function_t<int>* cb;
        ptr_t<int> out;
        int state =0;
        pthread_t id;
        int mode  =0;
    };  ptr_t<NODE> obj;

public: worker_t() noexcept : obj( new NODE ) {}

    virtual ~worker_t() noexcept {
        if( obj.count() > 1 ){ return; } 
        if( obj->state == 0 ){ return; } 
    //      force_close();
    }
    
    /*─······································································─*/

    template< class T, class... V >
    worker_t( T cb, const V&... arg ) noexcept : obj( new NODE() ){
        ptr_t<type::pair<bool,T>> pb = new type::pair<bool,T>({ 0, cb });
        ptr_t<int>               out = new int(1);
        obj->cb = new function_t<int>([=](){ 
            if( pb->first ){ return 1; }     pb->first = 1;
            int rs = ( pb->second )(arg...); pb->first = 0;
            return *out!=1 ? -1 : rs; 
        }); obj->out = out;
    }
    
    /*─······································································─*/

    int    pid() const noexcept { return (int)obj->id; }

    void close() const noexcept { *obj->out = 0; }
    
    /*─······································································─*/

    int detach() const noexcept { if( obj->state == 1 ){ return 0; } obj->state = 1;
        auto  pth = pthread_create( &obj->id, NULL, &dfunc, (void*)obj->cb );
        pthread_detach( obj->id ); return pth != 0 ? -1 : 0;
    }

    int join() const noexcept { if( obj->state == 1 ){ return 0; } obj->state = 1;
        auto  pth = pthread_create( &obj->id, NULL, &jfunc, (void*)obj->cb );
        pthread_join( obj->id, NULL ); return pth != 0 ? -1 : 0;
    }

    int add() const noexcept { if( obj->state == 1 ){ return 0; } obj->state = 1;
        auto pth = pthread_create( &obj->id, NULL, &sfunc, (void*)obj->cb );
        if( !pth ) { process::threads++; } pthread_detach( obj->id );
        return pth != 0 ? -1 : 0;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace worker { template< class... T >
    worker_t add( const T&... args ){ worker_t wrk( args... ); wrk.add(); return wrk; }
}}
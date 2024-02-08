#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>
#include <processthreadsapi.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace worker {
    void delay( ulong time ){ process::delay(time); }
    int    pid(){ return GetCurrentThreadId(); }
    void yield(){ process::delay(0); }
    void  exit(){ ExitThread(0); }
}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class mutex_t {
protected:

    ptr_t<HANDLE> mutex;

public:

    int lock(){ 
        auto   x  = WaitForSingleObject( &mutex, INFINITE );
        return x != WAIT_OBJECT_0;
    }

    int unlock(){ return ReleaseMutex( &mutex ); }

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

namespace nodepp { namespace { mutex_t mtx;

    DWORD WINAPI sfunc( LPVOID arg ){
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ worker::yield(); }
        mtx.lock(); process::threads--; mtx.unlock();
        delete cb; worker::exit(); return 0; 
    }

    DWORD WINAPI dfunc( LPVOID arg ){
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ worker::yield(); }
        delete cb; worker::exit(); return 0;
    }

    DWORD WINAPI jfunc( LPVOID arg ){
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ worker::yield(); }
        delete cb; worker::exit(); return 0;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class worker_t { 
protected:

    struct _str_ {
        function_t<int>* cb;
        HANDLE thread;
        ptr_t<int> out;
        int state = 0;
        DWORD  id;
    };  ptr_t<_str_> obj;

public: worker_t() noexcept : obj( new _str_ ) {}

    virtual ~worker_t() noexcept {
        if( obj.count() > 1 ){ return; } 
        if( obj->state == 0 ){ return; } 
        CloseHandle( obj->thread );
    }
    
    /*─······································································─*/

    template< class T, class... V >
    worker_t( T cb, V&... arg ) noexcept : obj( new _str_() ){
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

    int detach() const noexcept { if( obj->state == 1 ){ return 0; } int c = 0; obj->state = 1;
        obj->thread = CreateThread( NULL, 0, &dfunc, (void*)obj->cb, 0, &obj->id );
        WaitForSingleObject( obj->thread, 0 ); return obj->thread == NULL ? -1 : 0;
    }

    int join() const noexcept { if( obj->state == 1 ){ return 0; } int c = 0; obj->state = 1;
        obj->thread = CreateThread( NULL, 0, &jfunc, (void*)obj->cb, 0, &obj->id );
        WaitForSingleObject( obj->thread, INFINITE ); return obj->thread == NULL ? -1 : 0;
    }

    int add() const noexcept { if( obj->state == 1 ){ return 0; } int c = 0; obj->state = 1;
        obj->thread = CreateThread( NULL, 0, &sfunc, (void*)obj->cb, 0, &obj->id );
        if( obj->thread != NULL ){ process::threads++; }
        WaitForSingleObject( obj->thread, 0 ); return obj->thread == NULL ? -1 : 0;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace worker { template< class... T >
    worker_t add( const T&... args ){ worker_t wrk( args... ); wrk.add(); return wrk; }
}}

/*────────────────────────────────────────────────────────────────────────────*/
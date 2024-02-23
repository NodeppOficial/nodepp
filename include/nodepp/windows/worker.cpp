#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include "mutex.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace { 
    
    mutex_t mtx;

    DWORD WINAPI sfunc( LPVOID arg ){
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ worker::yield(); }
        mtx.lock(); process::threads--; mtx.unlock();
        delete cb; worker::exit(); return 0; 
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class worker_t { 
protected:

    struct NODE {
        function_t<int>* cb;
        HANDLE thread;
        ptr_t<int> out;
        int state = 0;
        DWORD  id;
    };  ptr_t<NODE> obj;

public: worker_t() noexcept : obj( new NODE ) {}

    virtual ~worker_t() noexcept {
        if( obj.count() > 1 ){ return; } 
        if( obj->state == 0 ){ return; } 
        CloseHandle( obj->thread );
    }
    
    /*─······································································─*/

    template< class T, class... V >
    worker_t( T cb, V&... arg ) noexcept : obj( new NODE() ){
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

    int run() const noexcept { if( obj->state == 1 ){ return 0; } int c = 0; obj->state = 1;
        obj->thread = CreateThread( NULL, 0, &sfunc, (void*)obj->cb, 0, &obj->id );
        if( obj->thread != NULL ){ process::threads++; }
        WaitForSingleObject( obj->thread, 0 ); return obj->thread == NULL ? -1 : 0;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
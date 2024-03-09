/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.nodepp.xyz/license.html
 */

/*────────────────────────────────────────────────────────────────────────────*/

#pragma once
#include "mutex.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace { 
    
    mutex_t mtx;

    void* sfunc( void* arg ){
        auto cb = (function_t<int>*) arg;
        while((*cb)() >= 0 ){ worker::yield(); }
        mtx.lock(); process::threads--; mtx.unlock();
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

    int run() const noexcept { if( obj->state == 1 ){ return 0; } obj->state = 1;
        auto pth = pthread_create( &obj->id, NULL, &sfunc, (void*)obj->cb );
        if( !pth ) { process::threads++; } pthread_detach( obj->id );
        return pth != 0 ? -1 : 0;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#pragma once

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

namespace { using KPOLLFD = struct kevent; }

class poll_t {
protected:

    struct NODE {
        ptr_t<int>     ls;
        ptr_t<KPOLLFD> ev;
        int            pd;
    };  ptr_t<NODE>   obj;

public:

    event_t<int>   onWrite;
    event_t<int>   onError;
    event_t<int>   onRead;

public: 

    poll_t() : obj( new NODE() ) {
        obj->pd = kqueue(); if( obj->pd == -1 )
        process::error("Can't open an epoll fd");
        obj->ev.resize( MAX_FILENO );
    }

    virtual ~poll_t() noexcept { 
        if( obj.count() > 1 ){ return; }
            close( obj->pd );
    }

    /*─······································································─*/

    ptr_t<int> get_last_poll() const noexcept { 
        ptr_t<int> result = obj->ls; return result; 
    }

    /*─······································································─*/

    int emit () noexcept { 
        static int c; static KPOLLFD x;
    coStart

        if((c=kevent( obj->pd, NULL, 0, &obj->ev, obj->ev.size(), 0 ))<=0 ) { coEnd; }

        while( c-->0 ){ x = obj->ev[c]; 
              if( x.flags & EV_ERROR     ){ onError.emit(x.ident); obj->ls={{-1, x.ident }}; coNext; }
            elif( x.flags & EVFILT_READ  ){  onRead.emit(x.ident); obj->ls={{ 0, x.ident }}; coNext; }
            elif( x.flags & EVFILT_WRITE ){ onWrite.emit(x.ident); obj->ls={{ 1, x.ident }}; coNext; }
        }

    coStop
    };

    /*─······································································─*/

    void push_write( const int& fd ) noexcept { KPOLLFD event;
	    EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL );
        kevent( obj->pd, &event, 1, NULL, 0, NULL );
    }

    void push_read( const int& fd ) noexcept { KPOLLFD event;
	    EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL );
        kevent( obj->pd, &event, 1, NULL, 0, NULL );
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
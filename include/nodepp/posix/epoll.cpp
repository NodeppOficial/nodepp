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
#include <sys/epoll.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

namespace { using EPOLLFD = struct epoll_event; }

class poll_t {
protected:

    struct NODE {
        ptr_t<int>     ls;
        ptr_t<EPOLLFD> ev;
        int            pd;
    };  ptr_t<NODE>   obj;

public:

    event_t<int>   onWrite;
    event_t<int>   onError;
    event_t<int>   onRead;

public: 

    poll_t() : obj( new NODE() ) {
        obj->pd = epoll_create1(0); if( obj->pd == -1 )
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
        static int c; static EPOLLFD x;
    coStart

        if((c=epoll_wait( obj->pd, &obj->ev, obj->ev.size(), 0 ))<=0 ) { coEnd; }

        while( c-->0 ){ x = obj->ev[c]; 
              if( x.events & EPOLLERR ){ onError.emit(x.data.fd); obj->ls={{-1, x.data.fd }}; coNext; }
            elif( x.events & EPOLLIN  ){  onRead.emit(x.data.fd); obj->ls={{ 0, x.data.fd }}; coNext; }
            elif( x.events & EPOLLOUT ){ onWrite.emit(x.data.fd); obj->ls={{ 1, x.data.fd }}; coNext; }
        }

    coStop
    };

    /*─······································································─*/

    void push_write( const int& fd ) noexcept { 
        EPOLLFD event; event.data.fd = fd;
        event.events = EPOLLOUT | EPOLLONESHOT;
        epoll_ctl( obj->pd, EPOLL_CTL_ADD, fd, &event );
    }

    void push_read( const int& fd ) noexcept { 
        EPOLLFD event; event.data.fd = fd;
        event.events = EPOLLIN | EPOLLONESHOT;
        epoll_ctl( obj->pd, EPOLL_CTL_ADD, fd, &event );
    }

};

}

/*────────────────────────────────────────────────────────────────────────────*/
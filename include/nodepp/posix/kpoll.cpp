#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

namespace { using KPOLLFD = struct kevent; }

class poll_t : public NODEPP_GENERATOR_BASE {
protected:

    struct _str_ {
        ptr_t<int>     ls;
        ptr_t<KPOLLFD> ev;
        int            pd;
    };  ptr_t<_str_>   obj;

public:

    event_t<int>   onWrite;
    event_t<int>   onError;
    event_t<int>   onRead;

public: 

    poll_t() : obj( new _str_() ) {
        obj->pd = kqueue(); if( obj->pd == -1 )
                 _Error("Can't open an epoll fd");
        obj->ev.resize( MAX_SOCKET );
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
    _GStart

        if((c=kevent( obj->pd, NULL, 0, &obj->ev, obj->ev.size(), 0 ))<=0 ) { _End; }

        while( c-->0 ){ x = obj->ev[c]; 
              if( x.flags & EV_ERROR     ){ onError.emit(x.ident); obj->ls={{-1, x.ident }}; _Next; }
            elif( x.flags & EVFILT_READ  ){  onRead.emit(x.ident); obj->ls={{ 0, x.ident }}; _Next; }
            elif( x.flags & EVFILT_WRITE ){ onWrite.emit(x.ident); obj->ls={{ 1, x.ident }}; _Next; }
        }

    _GStop
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
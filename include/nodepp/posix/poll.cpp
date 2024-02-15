#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <sys/poll.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

namespace { using POLLFD = struct pollfd; }

class poll_t : public NODEPP_GENERATOR_BASE {
protected:

    struct NODE {
        array_t<POLLFD> ev;
        ptr_t<int>      ls;
    };  ptr_t<NODE>    obj;

public:

    event_t<int>    onWrite;
    event_t<int>    onError;
    event_t<int>    onRead;

public: poll_t() noexcept : obj( new NODE() ) {}

    virtual ~poll_t() noexcept { 
        if ( obj.count() > 1 ){ return; }
        for( auto x : obj->ev ) onError.emit(x.fd); 
    }

    /*─······································································─*/

    ptr_t<int> get_last_poll() const noexcept { 
        ptr_t<int> result = obj->ls; return result; 
    }

    /*─······································································─*/

    int emit () noexcept { 
        static ulong s=0; static POLLFD x;
    gnStart

        if( obj->ev.empty() )                               { coEnd; }
        if( ::poll( obj->ev.data(), obj->ev.size(), 0 )<=0 ){ coEnd; } s = obj->ev.size(); 

        while( s-->0 ){ x = obj->ev[s]; 
              if( x.revents & POLLERR ){ obj->ev.erase(s); onError.emit(x.fd); obj->ls={{-1, x.fd }}; coNext; }
            elif( x.revents & POLLIN  ){ obj->ev.erase(s);  onRead.emit(x.fd); obj->ls={{ 0, x.fd }}; coNext; }
            elif( x.revents & POLLOUT ){ obj->ev.erase(s); onWrite.emit(x.fd); obj->ls={{ 1, x.fd }}; coNext; }
        }

    gnStop
    };

    /*─······································································─*/

    void push_write( const int& fd ) noexcept { 
	     obj->ev.unshift({ fd, POLLOUT, 0 }); 
    }

    void push_read( const int& fd ) noexcept { 
         obj->ev.unshift({ fd, POLLIN, 0 }); 
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <winsock.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

namespace { using POLLFD = WSAPOLLFD; }

class poll_t : public NODEPP_GENERATOR_BASE {
protected:

    struct _str_ {
        array_t<POLLFD> ev;
        ptr_t<int>      ls;
    };  ptr_t<_str_>    obj;


public:

    event_t<int>    onWrite;
    event_t<int>    onError;
    event_t<int>    onRead;

public: poll_t() noexcept : obj( new _str_() ) {}

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

        if( obj->ev.empty() )                                  { coEnd; }
        if( ::WSAPoll( obj->ev.data(), obj->ev.size(), 0 )<=0 ){ coEnd; } s = obj->ev.size(); 

        while( s-->0 ){ x = obj->ev[s]; 
              if( x.revents & POLLERR ){ obj->ev.erase(s); onError.emit(x.fd); obj->ls={{-1, (int)x.fd }}; coNext; }
            elif( x.revents & POLLIN  ){ obj->ev.erase(s);  onRead.emit(x.fd); obj->ls={{ 0, (int)x.fd }}; coNext; }
            elif( x.revents & POLLOUT ){ obj->ev.erase(s); onWrite.emit(x.fd); obj->ls={{ 1, (int)x.fd }}; coNext; }
        }
    
    gnStop
    };
    /*─······································································─*/

    void push_write( const SOCKET& fd ) noexcept { 
	     obj->ev.unshift({ fd, POLLOUT, 0 }); 
    }

    void push_read( const SOCKET& fd ) noexcept { 
         obj->ev.unshift({ fd, POLLIN, 0 }); 
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
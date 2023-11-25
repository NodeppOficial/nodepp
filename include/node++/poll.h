#ifndef NODEPP_POLL
#define NODEPP_POLL

/*────────────────────────────────────────────────────────────────────────────*/

#include "event.h"
#include <poll.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

namespace { using POLLFD = struct pollfd; }

/*────────────────────────────────────────────────────────────────────────────*/

namespace _poll_{ struct str {
    array_t<POLLFD> poll;
};}

/*────────────────────────────────────────────────────────────────────────────*/

class poll_t {
protected:
    ptr_t<_poll_::str> obj = new _poll_::str();

public:

    event_t<int>    onWrite;
    event_t<int>    onError;
    event_t<int>    onRead;

    /*─······································································─*/

    poll_t() noexcept = default;

    virtual ~poll_t() noexcept { 
         if( obj.count() > 1 ){ return; }
        for( auto x : obj->poll ) onError.emit(x.fd); 
    }

    /*─······································································─*/

    bool has_fd( int fd ) const noexcept {
        for( auto x : obj->poll ) if( x.fd == fd ) 
           { return 1; } return 0;
    }

    ulong size() const noexcept {
        return obj->poll.size();
    }

    /*─······································································─*/

    ptr_t<int> _emit ( const ulong& timeout=0 ) noexcept { static ulong s=0;

        if( obj->poll.empty() )                                       { return nullptr; }
        
        if( ::poll( obj->poll.data(), obj->poll.size(), timeout )<=0 ){ return nullptr; }

        while( s-->0 ){ auto x = obj->poll[s]; if(
                x.revents == POLLNVAL || x.revents == POLLERR ||
                x.revents == POLLHUP
            )                              { obj->poll.erase(s); return {{-1, x.fd }}; }
            else if( x.revents == POLLIN  ){ obj->poll.erase(s); return {{ 0, x.fd }}; }
            else if( x.revents == POLLOUT ){ obj->poll.erase(s); return {{ 1, x.fd }}; }
        }  
        
        s = obj->poll.size(); return nullptr;
    };

    /*─······································································─*/

    int emit ( const ulong& timeout=0 ) noexcept {
        static POLLFD x; static ulong s=0; _Start

        if( obj->poll.empty() )                                       { _Next; }
        
        if( ::poll( obj->poll.data(), obj->poll.size(), timeout )<=0 ){ _Next; }

        while( s-->0 ){ x = obj->poll[s]; if(
                x.revents == POLLNVAL || x.revents == POLLERR ||
                x.revents == POLLHUP
            )                              { obj->poll.erase(s); onError.emit(x.fd); _Next; }
            else if( x.revents == POLLIN  ){ obj->poll.erase(s);  onRead.emit(x.fd); _Next; }
            else if( x.revents == POLLOUT ){ obj->poll.erase(s); onWrite.emit(x.fd); _Next; }
        }
        
        s = obj->poll.size(); _Stop
    };
    /*─······································································─*/

    void push_write( const int& fd ) noexcept { 
	     obj->poll.unshift({ fd, POLLOUT, 0 }); 
    }

    void push_read( const int& fd ) noexcept { 
         obj->poll.unshift({ fd, POLLIN, 0 }); 
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
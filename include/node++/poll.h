#ifndef NODEPP_POLL
#define NODEPP_POLL

/*────────────────────────────────────────────────────────────────────────────*/

#include "event.h"
#include <poll.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

namespace { using POLLFD = struct pollfd; }

/*────────────────────────────────────────────────────────────────────────────*/

class poll_t {
protected:

    array_t<POLLFD> poll;

public:

    event_t<int>    onWrite;
    event_t<int>    onError;
    event_t<int>    onRead;

    /*─······································································─*/

    poll_t() noexcept = default;

    virtual ~poll_t() noexcept { 
         if( poll.ptr().count() > 1 ){ return; }
        for( auto x : poll ) onError.emit(x.fd); 
    }

    /*─······································································─*/

    bool has_fd( int fd ) const noexcept {
        for( auto x : poll ) if( x.fd == fd ) 
           { return 1; } return 0;
    }

    ulong size() const noexcept {
        return poll.size();
    }

    /*─······································································─*/

    ptr_t<int> _emit ( const ulong& timeout=0 ) noexcept { static ulong s=0;

        if( poll.empty() )                                       { return nullptr; }
        
        if( ::poll( poll.data(), poll.size(), timeout )<=0 ){ return nullptr; }

        while( s-->0 ){ auto x = poll[s]; if(
                x.revents == POLLNVAL || x.revents == POLLERR ||
                x.revents == POLLHUP
            )                              { poll.erase(s); return {{-1, x.fd }}; }
            else if( x.revents == POLLIN  ){ poll.erase(s); return {{ 0, x.fd }}; }
            else if( x.revents == POLLOUT ){ poll.erase(s); return {{ 1, x.fd }}; }
        }  
        
        s = poll.size(); return nullptr;
    };

    /*─······································································─*/

    int emit ( const ulong& timeout=0 ) noexcept {
        static POLLFD x; static ulong s=0; _Start

        if( poll.empty() )                                       { _Next; }
        
        if( ::poll( poll.data(), poll.size(), timeout )<=0 ){ _Next; }

        while( s-->0 ){ x = poll[s]; if(
                x.revents == POLLNVAL || x.revents == POLLERR ||
                x.revents == POLLHUP
            )                              { poll.erase(s); onError.emit(x.fd); _Next; }
            else if( x.revents == POLLIN  ){ poll.erase(s);  onRead.emit(x.fd); _Next; }
            else if( x.revents == POLLOUT ){ poll.erase(s); onWrite.emit(x.fd); _Next; }
        }
        
        s = poll.size(); _Stop
    };
    /*─······································································─*/

    void push_write( const int& fd ) noexcept { 
	     poll.unshift({ fd, POLLOUT, 0 }); 
    }

    void push_read( const int& fd ) noexcept { 
         poll.unshift({ fd, POLLIN, 0 }); 
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
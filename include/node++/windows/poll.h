#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <winsock.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

class poll_t {
protected:

    array_t<WSAPOLLFD> poll;

public:

    event_t<int>    onWrite;
    event_t<int>    onError;
    event_t<int>    onRead;

public: poll_t() noexcept {}

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

        if( poll.empty() )                                     { return nullptr; }
        
        if( ::WSAPoll( poll.data(), poll.size(), timeout )<=0 ){ return nullptr; }

        while( s-->0 ){ auto x = poll[s]; if(
                x.revents == POLLNVAL || x.revents == POLLERR ||
                x.revents == POLLHUP
            )                              { poll.erase(s); return {{ -1, (int)x.fd }}; }
            else if( x.revents == POLLIN  ){ poll.erase(s); return {{  0, (int)x.fd }}; }
            else if( x.revents == POLLOUT ){ poll.erase(s); return {{  1, (int)x.fd }}; }
        }  
        
        s = poll.size(); return nullptr;
    };

    /*─······································································─*/

    int emit ( const ulong& timeout=0 ) noexcept {
        static WSAPOLLFD x; static ulong s=0; $Start

        if( poll.empty() )                                     { $Next; }
        
        if( ::WSAPoll( poll.data(), poll.size(), timeout )<=0 ){ $Next; }

        while( s-->0 ){ x = poll[s]; if(
                x.revents == POLLNVAL || x.revents == POLLERR ||
                x.revents == POLLHUP
            )                              { poll.erase(s); onError.emit(x.fd); $Next; }
            else if( x.revents == POLLIN  ){ poll.erase(s);  onRead.emit(x.fd); $Next; }
            else if( x.revents == POLLOUT ){ poll.erase(s); onWrite.emit(x.fd); $Next; }
        }
        
        s = poll.size(); $Stop
    };
    /*─······································································─*/

    void push_write( const SOCKET& fd ) noexcept { 
	     poll.unshift({ fd, POLLOUT, 0 }); 
    }

    void push_read( const SOCKET& fd ) noexcept { 
         poll.unshift({ fd, POLLIN, 0 }); 
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
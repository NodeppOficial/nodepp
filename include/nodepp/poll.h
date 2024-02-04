#ifndef NODEPP_POLL
#define NODEPP_POLL

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
        #include "event.h"
        #include "windows/poll.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
    #if   _POLL == NODEPP_POLL_EPOLL
        #include "event.h"
        #include "posix/epoll.cpp"
    #elif _POLL == NODEPP_POLL_KPOLL
        #include "event.h"
        #include "posix/kpoll.cpp"
    #else
        #include "event.h"
        #include "posix/poll.cpp"
    #endif
#else
    #define NODEPP_NO_POLL
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/

#ifdef NODEPP_NO_POLL

namespace nodepp {

namespace { using POLLFD = struct pollfd; }

class poll_t : public NODEPP_GENERATOR_BASE {
protected:

    struct POLLFD {
        int fd;
        int md;
    };

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
        for( auto x : obj->ev ) onError.emit( x.md ); 
    }

    /*─······································································─*/

    ptr_t<int> get_last_poll() const noexcept { 
        ptr_t<int> result = obj->ls; return result; 
    }

    /*─······································································─*/

    int emit () noexcept { 
        static ulong s = 0; static POLLFD x;
    _GStart

        while ( s-->0 ) { x = obj->ev[s];
            if( x.md == 1 ){ obj->ev.erase(s); onWrite.emit(x.fd); obj->ls={{ 1, x.fd }}; _Next; }
          elif( x.md == 0 ){ obj->ev.erase(s);  onRead.emit(x.fd); obj->ls={{ 0, x.fd }}; _Next; }
          else             { obj->ev.erase(s); onError.emit(x.fd); obj->ls={{-1, x.fd }}; _Next; }
        } s = obj->ev.size();

    _GStop
    };

    /*─······································································─*/

    void push_write( const int& fd ) noexcept { 
	     obj->ev.unshift({ fd, 1 }); 
    }

    void push_read( const int& fd ) noexcept { 
         obj->ev.unshift({ fd, 0 }); 
    }

};}

#undef NODEPP_NO_POLL
#endif
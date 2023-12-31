#ifndef NODEPP_UDP
#define NODEPP_UDP

/*────────────────────────────────────────────────────────────────────────────*/

#include "socket.h"
#include "poll.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

class udp_t {
protected:

    struct _str_ {
        agent_t*                  agent = nullptr;
        int                       state = 0;
        function_t<void,socket_t> func;
    };  ptr_t<_str_> obj;
    
public: udp_t() noexcept : obj( new _str_() ) {}

    event_t<socket_t>         onConnect;
    event_t<socket_t>         onSocket;
    event_t<>                 onClose;
    event_t<except_t>         onError;
    event_t<socket_t>         onOpen;
    
    /*─······································································─*/

    udp_t( decltype(obj->func) _func, agent_t* opt=nullptr ) noexcept : obj( new _str_() )
         { obj->agent = opt; obj->func = _func; }
    
    /*─······································································─*/
    
    void     close() const noexcept { if( obj->state<0 ){ return; } obj->state=-1; onClose.emit(); }
    bool is_closed() const noexcept { return obj == nullptr ? 1 : obj->state < 0; }
    
    /*─······································································─*/

    void listen( const string_t& host, int port, decltype(obj->func)* cb=nullptr ) const noexcept {
        if( obj->state == 1 ) { return; } obj->state = 1;
            ptr_t<udp_t> self = new udp_t( *this );

        socket_t sk = socket_t(); 
                 sk.PROT = IPPROTO_UDP;
                 sk.SOCK = SOCK_DGRAM;
                 sk.socket(host,port);
                 sk.set_sockopt(obj->agent);
        
        if( sk.bind() < 0 ){ $onError(onError,"Error while binding UDP"); close(); return; }
        if( cb != nullptr ){ (*cb)(sk); } sk.onClose.on([=](){ self->close(); });
        onOpen.emit(sk); sk.onOpen.emit(); onSocket.emit(sk); obj->func(sk);
    }

    void listen( const string_t& host, int port, decltype(obj->func) cb ) const noexcept { 
         listen( host, port, &cb );
    }
    
    /*─······································································─*/

    void connect( const string_t& host, int port, decltype(obj->func)* cb=nullptr ) const noexcept {
        if( obj->state == 1 ){ return; } obj->state = 1;
        ptr_t<udp_t> self = new udp_t( *this );

        socket_t sk = socket_t(); 
                 sk.PROT = IPPROTO_UDP;
                 sk.SOCK = SOCK_DGRAM;
                 sk.socket(host,port);  
                 sk.set_sockopt(obj->agent);
        
    //  if( sk.connect() < 0 ){ $onError(onError,"Error while accepting UDP"); close(); return;  }
        if( cb != nullptr ){ (*cb)(sk); } sk.onClose.on([=](){ self->close(); });
        onOpen.emit(sk); sk.onOpen.emit(); onSocket.emit(sk); obj->func(sk);
    }

    void connect( const string_t& host, int port, decltype(obj->func) cb ) const noexcept { 
         connect( host, port, &cb ); 
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

namespace udp {
    template< class U > udp_t server( U func, agent_t* opt=nullptr ){ return udp_t(func,opt); }
    template< class U > udp_t client( U func, agent_t* opt=nullptr ){ return udp_t(func,opt); }
}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
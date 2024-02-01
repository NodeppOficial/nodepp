#ifndef NODEPP_TCP
#define NODEPP_TCP

/*────────────────────────────────────────────────────────────────────────────*/

#include "socket.h"
#include "poll.h"
#include "dns.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

class tcp_t {
protected:

    struct _str_ {
        int                       state = 0;
        bool                      chck  = 1;
        agent_t                   agent;
        poll_t                    poll ;
        function_t<void,socket_t> func ;
    };  ptr_t<_str_> obj;
    
    /*─······································································─*/

    void init_poll_loop() const noexcept { process::poll::add([=]( tcp_t inp ){
        if( inp.is_closed() ){ return -1; } if( inp.obj->poll.emit() != -1 ) { auto x = inp.obj->poll.get_last_poll();
            if( x[0] == 0 ){ socket_t cli(x[1]); if(cli.is_available()){ cli.set_sockopt(inp.obj->agent); inp.onSocket.emit(cli); inp.obj->func(cli); }}
            if( x[0] == 1 ){ socket_t cli(x[1]); if(cli.is_available()){ cli.set_sockopt(inp.obj->agent); inp.onSocket.emit(cli); inp.obj->func(cli); }}
        #if _KERNEL == NODEPP_KERNEL_WINDOWS
            if( x[0] ==-1 ){ ::closesocket(x[1]); }
        #else
            if( x[0] ==-1 ){ ::close(x[1]); }
        #endif
        }   return 1;
    }, *this ); }
    
public: tcp_t() noexcept : obj( new _str_() ) {}

    event_t<socket_t> onConnect;
    event_t<socket_t> onSocket;
    event_t<>         onClose;
    event_t<except_t> onError;
    event_t<socket_t> onOpen;
    
    /*─······································································─*/
    
    tcp_t( decltype(obj->func) _func, agent_t* opt=nullptr ) noexcept : obj( new _str_() )
         { obj->func=_func; obj->agent=opt==nullptr?agent_t():*opt;  }
    
    /*─······································································─*/
    
    void     close() const noexcept { if( obj->state<0 ){ return; } obj->state=-1; onClose.emit(); }
    bool is_closed() const noexcept { return obj == nullptr ? 1 : obj->state < 0; }
    
    /*─······································································─*/

    void poll( bool chck ) const noexcept { obj->chck = chck; }
    
    /*─······································································─*/

    void listen( const string_t& host, int port, decltype(obj->func)* cb=nullptr ) const noexcept {
        if( obj->state == 1 ){ return; } obj->state = 1;
        if( dns::lookup(host).empty() ){ _EError(onError,"dns couldn't get ip"); close(); return; }

        socket_t *sk = new socket_t; 
                  sk->PROT = IPPROTO_TCP;
                  sk->socket( dns::lookup(host), port ); 
        
        if(   sk->bind() < 0 ){ _EError(onError,"Error while binding TCP"); close(); delete sk; return; }
        if( sk->listen() < 0 ){ _EError(onError,"Error while listening TCP"); close(); delete sk; return; }
        if( obj->chck == true ){ init_poll_loop(); }

        onOpen.emit(*sk); if( cb != nullptr ){ (*cb)(*sk); }
        
        process::task::add([=]( tcp_t inp ){
            static int _accept = 0; 
        _Start

            while( sk != nullptr ){ _accept = sk->_accept();
                if( inp.is_closed() || !sk->is_available() )
                  { break; } elif ( _accept != -2 )
                  { break; } _Yield(1);
            }
            
            if( _accept == -1 ){ _EError(inp.onError,"Error while accepting TCP"); _Goto(2); }
            elif ( !sk->is_available() || inp.is_closed() ){ _Goto(2); }
            elif ( inp.obj->chck == true ){ inp.obj->poll.push_read(_accept); _Goto(0); }
            else { socket_t cli( _accept ); if( cli.is_available() ){ 
                   process::poll::add([]( socket_t cli, tcp_t inp ){
                        cli.set_sockopt( inp.obj->agent ); 
                        inp.onSocket.emit( cli ); 
                        inp.obj->func( cli ); 
                        return -1;
                   }, cli, inp );
            } _Goto(0); }

            _Yield(2); inp.close(); delete sk; 
        
        _Stop
        }, *this );

    }

    void listen( const string_t& host, int port, decltype(obj->func) cb ) const noexcept { 
         listen( host, port, &cb ); 
    }

    /*─······································································─*/

    void connect( const string_t& host, int port, decltype(obj->func)* cb=nullptr ) const noexcept {
        if( obj->state == 1 ){ return; } obj->state = 1;
        if( dns::lookup(host).empty() ){ _EError(onError,"dns couldn't get ip"); close(); return; }
            ptr_t<tcp_t> self = new tcp_t( *this );

        socket_t sk = socket_t(); 
                 sk.PROT = IPPROTO_TCP;
                 sk.socket( dns::lookup(host), port );
                 sk.set_sockopt( obj->agent );

        if( sk.connect() < 0 ){ _EError(onError,"Error while connecting TCP"); close(); return; }
        if( cb != nullptr ){ (*cb)(sk); } sk.onClose.on([=](){ self->close(); });
        onOpen.emit(sk); sk.onOpen.emit(); onSocket.emit(sk); obj->func(sk);
    }

    void connect( const string_t& host, int port, decltype(obj->func) cb ) const noexcept { 
         connect( host, port, &cb ); 
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

namespace tcp {

    tcp_t server( const tcp_t& server ){ server.onSocket([=]( socket_t cli ){
        ptr_t<_file_::read> _read = new _file_::read;

        server.onConnect([=]( socket_t cli ){ process::poll::add([=](){
            if(!cli.is_available() ) { cli.close(); return -1; }
            if((*_read)(&cli)==1 )   { return 1; }
            if(  _read->c  <=  0 )   { return 1; }
            cli.onData.emit(_read->y); return 1;
        }) ; });

        process::task::add([=](){
            server.onConnect.emit(cli); return -1;
        }); cli.onDrain([=](){ cli.free(); });

    }); server.poll( false ); return server; }

    /*─······································································─*/

    tcp_t server( agent_t* opt=nullptr ){
        auto server = (tcp_t){ [=]( socket_t cli ){}, opt };
        tcp::server( server ); return server; 
    }

    /*─······································································─*/

    tcp_t client( const tcp_t& client ){ client.onOpen([=]( socket_t cli ){
        ptr_t<_file_::read> _read = new _file_::read;

        process::poll::add([=](){
            if(!cli.is_available() ) { cli.close(); return -1; }
            if((*_read)(&cli)==1 )   { return 1; }
            if(  _read->c  <=  0 )   { return 1; }
            cli.onData.emit(_read->y); return 1;
        }); cli.onDrain([=](){ cli.free(); });

    }); return client; }

    /*─······································································─*/

    tcp_t client( agent_t* opt=nullptr ){
        auto client = (tcp_t){ [=]( socket_t cli ){}, opt };
        tcp::client( client ); return client; 
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
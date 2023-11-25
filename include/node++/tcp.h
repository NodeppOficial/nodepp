#ifndef NODEPP_TCP
#define NODEPP_TCP

/*────────────────────────────────────────────────────────────────────────────*/

#include "socket.h"
#include "poll.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace _tcp_ { struct str {
    agent_t*                  agent = nullptr;
    int                       state = 0;
    poll_t                    poll;
    function_t<void,socket_t> func;
};}

/*────────────────────────────────────────────────────────────────────────────*/

class tcp_t {
protected:

    ptr_t<_tcp_::str> obj = new _tcp_::str();
    
    /*─······································································─*/

    void init_poll_loop() const noexcept { process::task::add([=]( tcp_t inp ){
        if( inp.is_closed() ){ return -1; } auto x = inp.obj->poll._emit(); if( x != nullptr ) {
            if( x[0] == 0 ){ socket_t cli(x[1]); if(cli.is_available()){ cli.set_sockopt(inp.obj->agent); inp.onSocket.emit(cli); inp.obj->func(cli); }}
            if( x[0] == 1 ){ socket_t cli(x[1]); if(cli.is_available()){ cli.set_sockopt(inp.obj->agent); inp.onSocket.emit(cli); inp.obj->func(cli); }}
            if( x[0] ==-1 ){ ::close(x[1]); }
        }   return 1;
    }, *this ); }
    
public:

    event_t<socket_t> onConnect;
    event_t<socket_t> onSocket;
    event_t<>         onClose;
    event_t<except_t> onError;
    event_t<socket_t> onOpen;
    
    /*─······································································─*/
    
    tcp_t( decltype(obj->func) _func, agent_t* opt=nullptr ) noexcept 
         { obj->agent=opt; obj->func=_func; }
    
    /*─······································································─*/
    
    void     close() const noexcept { if( obj->state<0 ){ return; } obj->state=-1; onClose.emit(); }
    bool is_closed() const noexcept { return obj == nullptr ? 1 : obj->state < 0; }
    
    /*─······································································─*/

    void listen( string_t host, int port, decltype(obj->func)* cb=nullptr ) const noexcept {
        if( obj->state == 1 ){ return; } obj->state = 1;

        socket_t *sk = new socket_t; sk->socket( host, port ); 
        
        if(   sk->bind() < 0 ){ _onError(onError,"Error while binding TCP"); close(); delete sk; return; }
        if( sk->listen() < 0 ){ _onError(onError,"Error while listening TCP"); close(); delete sk; return; }
        
        onOpen.emit(*sk); if( cb != nullptr ) (*cb)(*sk); init_poll_loop(); 
        
        process::task::add([=]( tcp_t inp ){
            static int _accept=0; _Start

            while(( _accept=sk->_accept() )==-2 ){ 
                if( !sk->is_available() || inp.is_closed() )
                  { break; } _Yield(1);
            }
            
            if( _accept == -1 ){ _onError(inp.onError,"Error while accepting TCP"); _Goto(2); }
            else if( !sk->is_available() || inp.is_closed() ){ _Goto(2); }
            else { inp.obj->poll.push_read(_accept); _Goto(0); }

            _Yield(2); inp.close(); delete sk; _Stop
        }, *this );

    }

    void listen( string_t host, int port, decltype(obj->func) cb ) const noexcept { 
         listen( host, port, &cb ); 
    }

    /*─······································································─*/

    void connect( string_t host, int port, decltype(obj->func)* cb=nullptr ) const noexcept {
        if( obj->state == 1 ){ return; } obj->state = 1;
        ptr_t<tcp_t> self = new tcp_t( *this );

        socket_t sk = socket_t(); 
                 sk.socket( host, port );  
                 sk.set_sockopt( obj->agent );

        if( sk.connect() < 0 ){ _onError(onError,"Error while accepting TCP"); close(); return; }
        if( cb != nullptr ) (*cb)( sk ); sk.onClose.on([=](){ self->close(); });
        onOpen.emit(sk); sk.onOpen.emit(); onSocket.emit(sk); obj->func(sk);
    }

    void connect( string_t host, int port, decltype(obj->func) cb ) const noexcept { 
         connect( host, port, &cb ); 
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

namespace tcp {

    tcp_t server( const tcp_t& server ){ server.onSocket([=]( socket_t cli ){
        ptr_t<_file_::read> _read = new _file_::read;

        server.onConnect([=]( socket_t cli ){ process::task::add([=](){
            while(!cli.is_available() ){ cli.close(); return -1; }
            while((*_read)(&cli)==1 ){ return 1; } 
               if( _read->c<=0 )     { return 1; }
            cli.onData.emit(_read->y); return 1;
        });});

        process::task::add([=](){
            server.onConnect.emit(cli); return -1;
        }); cli.onDrain([=](){ cli.free(); });

    }); return server; }

    /*─······································································─*/

    tcp_t server( agent_t* opt=nullptr ){
        auto server = (tcp_t){ [=]( socket_t cli ){}, opt };
        tcp::server( server ); return server; 
    }

    /*─······································································─*/

    tcp_t client( const tcp_t& client ){ client.onOpen([=]( socket_t cli ){
        ptr_t<_file_::read> _read = new _file_::read;

        process::task::add([=](){
            while(!cli.is_available() ){ cli.close(); return -1; }
            while((*_read)(&cli)==1 ){ return 1; } 
               if( _read->c<=0 )     { return 1; }
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
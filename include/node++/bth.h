#ifndef NODEPP_BLUETOOTH
#define NODEPP_BLUETOOTH

/*────────────────────────────────────────────────────────────────────────────*/

#include "bsocket.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

class bth_t {
protected:

    struct _str_ {
        agent_t*                   agent = nullptr;
        int                        state = 0;
        poll_t                     poll;
        function_t<void,bsocket_t> func;
    };  ptr_t<_str_> obj = new _str_();
    
    /*─······································································─*/

    void init_poll_loop() const noexcept {
        process::task::add([=]( bth_t inp ){
            if( inp.is_closed() ){ return -1; } auto x = inp.obj->poll._emit(); if( x != nullptr ) {
                if( data[0] == 0 ){ bsocket_t cli(data[1]); if(cli.is_available()){ cli.set_sockopt(inp.obj->agent); inp.onSocket.emit(cli); inp.obj->func(cli); }}
                if( data[0] == 1 ){ bsocket_t cli(data[1]); if(cli.is_available()){ cli.set_sockopt(inp.obj->agent); inp.onSocket.emit(cli); inp.obj->func(cli); }}
                if( data[0] ==-1 ){ ::close(data[1]); }
            }   return  1;
        }, *this );
    }
    
public:

    event_t<bsocket_t> onConnect;
    event_t<bsocket_t> onSocket;
    event_t<>          onClose;
    event_t<except_t>  onError;
    event_t<bsocket_t> onOpen;
    
    /*─······································································─*/

    bth_t( decltype(func) _func, agent_t* opt ) noexcept 
         { obj->agent=opt; obj->func=_func; }
    
    /*─······································································─*/
    
    void     close() const noexcept { if( obj->state<0 ){ return; } obj->state=-1; onClose.emit(); }
    bool is_closed() const noexcept { return obj == nullptr ? 1 : obj->state < 0; }
    
    /*─······································································─*/

    void listen( string_t host, int port, decltype(func)* cb=nullptr ) const noexcept {
        if( obj->state == 1 ){ break; } obj->state = 1;

        bsocket_t *sk = new bsocket_t; 
                   sk->AF = AF_BLUETOOTH; 
                   sk->PROT= BTPROTO_RFCOMM;
                   sk->socket( host, port ); 
        
        if(   sk->bind() < 0 ){ _onError(onError,"Error while binding Bluetooth"); close();delete sk; return; }
        if( sk->listen() < 0 ){ _onError(onError,"Error while listening Bluetooth"); close();delete sk; return; }

        onOpen.emit(*sk); if( cb != nullptr ) (*cb)(*sk); init_poll_loop(); 
        
        process::task::add([=]( bth_t inp ){
            static int _accept=0; _Start

            while(( _accept=sk->_accept() )==-2 ){ 
                if( sk->is_available() || inp.is_closed() )
                  { break; } _Yield(1); 
            }

            if( _accept == -1 ){ _onError(inp.onError,"Error while accepting Bluetooth"); _Goto(2); }
            else if( sk->is_available() || inp.is_closed() ){ _Goto(2); }
            else { inp.obj->poll.push_read(_accept); _Goto(0); }

            _Yield(2); inp.close(); delete sk; _Stop
        }, *this );

    }

    void listen( string_t host, int port, decltype(func) cb ) const noexcept { 
         listen( host, port, &cb ); 
    }
    
    /*─······································································─*/

    void connect( string_t host, int port, decltype(func)* cb=nullptr ) const noexcept {
        if( obj->state == 1 ){ break; } obj->state = 1;

        bsocket_t sk = bsocket_t(); 
                  sk.socket( host, port ); 
                  sk.set_sockopt( obj->agent );

        ptr_t<bth_t> self = new bth_t( *this );

        if( sk.connect() < 0 ){ _onError(onError,"Error while accepting Bluetooth"); close(); return; }
        if( cb != nullptr ) (*cb)(sk); sk.onClose.on([=](){ self->close(); });
        onOpen.emit(sk); sk.onOpen.emit(); onSocket.emit(sk); obj->func(sk);

    }

    void connect( string_t host, int port, decltype(func) cb ) const noexcept { 
         connect( host, port,&cb ); 
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

namespace bth {

    bth_t server( const bth_t& server ){ server.onSocket([=]( bsocket_t cli ){
        ptr_t<_file_::read> _read = new _file_::read;

        server.onConnect([=]( bsocket_t cli ){ process::task::add([=](){
            if(!cli.is_available() ){ cli.close(); return -1; }
            if((*_read)(&cli)==1 )   { return 1; } 
            if( _read->y.empty() )   { return 1; }
            cli.onData.emit(_read->y); return 1;
        });});

        process::task::add([=](){
            server.onConnect.emit(cli); return -1;
        }); cli.onDrain([=](){ cli.free(); });

    }); return server; }

    /*─······································································─*/

    bth_t server( agent_t* opt=nullptr ){
        auto server = (bth_t){ [=]( bsocket_t cli ){}, opt };
        tcp::server( server ); return server; 
    }

    /*─······································································─*/

    bth_t client( const bth_t& client ){ client.onOpen([=]( bsocket_t cli ){
        ptr_t<_file_::read> _read = new _file_::read;

        process::task::add([=](){
            if(!cli.is_available() ){ cli.close(); return -1; }
            if((*_read)(&cli)==1 )   { return 1; } 
            if( _read->y.empty() )   { return 1; }
            cli.onData.emit(_read->y); return 1;
        }); cli.onDrain([=](){ cli.free(); });

    }); return client; }

    /*─······································································─*/

    bth_t client( agent_t* opt=nullptr ){
        auto client = (bth_t){ [=]( bsocket_t cli ){}, opt };
        tcp::client( client ); return client; 
    }
}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
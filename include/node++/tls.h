#ifndef NODEPP_TLS
#define NODEPP_TLS

/*────────────────────────────────────────────────────────────────────────────*/

#include "ssocket.h"
#include "poll.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

class tls_t {
protected:

    struct _str_ {
        agent_t*                   agent = nullptr;
        ssl_t*                     ctx   = nullptr; 
        int                        state = 0;
        poll_t                     poll;
        function_t<void,ssocket_t> func;
    };  ptr_t<_str_> obj;
    
    /*─······································································─*/

    void init_poll_loop() const noexcept { process::task::add([=]( tls_t inp ){
        if( inp.is_closed() ){ return -1; } auto x = inp.obj->poll._emit(); if( x != nullptr ) {
            if( x[0] == 0 ){ ssocket_t cli(inp.obj->ctx,x[1]); if(cli.is_available()){ cli.set_sockopt(inp.obj->agent); inp.onSocket.emit(cli); inp.obj->func(cli); }}
            if( x[0] == 1 ){ ssocket_t cli(inp.obj->ctx,x[1]); if(cli.is_available()){ cli.set_sockopt(inp.obj->agent); inp.onSocket.emit(cli); inp.obj->func(cli); }}
        #if $KERNEL == NODEPP_KERNEL_WINDOWS
            if( x[0] ==-1 ){ ::closesocket(x[1]); }
        #else
            if( x[0] ==-1 ){ ::close(x[1]); }
        #endif
        }   return 1; 
    }, *this ); }
    
public: tls_t() noexcept : obj( new _str_() ) {}

    event_t<ssocket_t> onConnect;
    event_t<ssocket_t> onSocket;
    event_t<>          onClose;
    event_t<except_t>  onError;
    event_t<ssocket_t> onOpen;
    
    /*─······································································─*/

    tls_t( decltype(obj->func) _func, ssl_t* xtc, agent_t* opt=nullptr ) noexcept : obj( new _str_() )
         { obj->agent = opt; obj->ctx = xtc; obj->func = _func; }

    /*─······································································─*/
    
    void     close() const noexcept { if( obj->state<0 ){ return; } obj->state=-1; onClose.emit(); }
    bool is_closed() const noexcept { return obj == nullptr ? 1 : obj->state < 0; }
    
    /*─······································································─*/

    void listen( const string_t& host, int port, decltype(obj->func)* cb=nullptr  ) const noexcept {
        if( obj->state == 1 ){ return; } obj->state = 1; if( obj->ctx == nullptr || obj->ctx->create_server() == -1 )
          { $onError(onError,"Error Initializing SSL context"); close(); return; }
        
        ssocket_t *sk = new ssocket_t; 
                   sk->PROT = IPPROTO_TCP;
                   sk->socket( host, port ); 
        
        if(   sk->bind() < 0 ){ $onError(onError,"Error while binding TLS"); close(); delete sk; return; }
        if( sk->listen() < 0 ){ $onError(onError,"Error while listening TLS"); close(); delete sk; return; }

        onOpen.emit(*sk); init_poll_loop(); if( cb != nullptr ){ (*cb)(*sk); } 
        
        process::task::add([=]( tls_t inp ){
            int _accept=0; $Start

            while(( _accept=sk->_accept() )==-2 ){
                if( !sk->is_available() || inp.is_closed() )
                  { break; } $Yield(1);
            }
            
            if( _accept == -1 ){ $onError(onError,"Error while accepting TLS"); $Goto(2); }
            else if( !sk->is_available() || inp.is_closed() ){ $Goto(2); }
            else { inp.obj->poll.push_read(_accept); $Goto(0); }

            $Yield(2); inp.close(); delete sk; $Stop
        }, *this );

    }

    void listen( const string_t& host, int port, decltype(obj->func) cb ) const noexcept { 
         listen( host, port, &cb ); 
    }
    
    /*─······································································─*/

    void connect( const string_t& host, int port, decltype(obj->func)* cb=nullptr  ) const noexcept {
        if( obj->state == 1 ){ return; } obj->state = 1; if( obj->ctx == nullptr || obj->ctx->create_client() < 0 )
          { $onError(onError,"Error Initializing SSL context"); close(); return; }
            ptr_t<tls_t> self = new tls_t( *this );

        ssocket_t sk = ssocket_t(); 
                  sk.PROT = IPPROTO_TCP;
                  sk.socket( host, port );
                  sk.set_sockopt( obj->agent );

        if( sk.connect() < 0 ){ 
            $onError(onError,"Error while connecting TLS"); 
            close(); return; 
        }

        sk.ssl = new ssl_t( obj->ctx, sk.get_fd() ); 

        if( sk.ssl->connect() <= 0 ){ 
            $onError(onError,"Error while handshaking TLS");
            close(); return; 
        }

        if( cb != nullptr ){ (*cb)(sk); }  sk.onClose.on([=](){ self->close(); });
        onOpen.emit(sk); sk.onOpen.emit(); onSocket.emit(sk); obj->func(sk); 
    }

    void connect( const string_t& host, int port, decltype(obj->func) cb ) const noexcept { 
         connect( host, port, &cb ); 
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

namespace tls {
    
    tls_t server( const tls_t& server ){ server.onSocket([=]( ssocket_t cli ){
        ptr_t<_file_::read> _read = new _file_::read;

        server.onConnect([=]( ssocket_t cli ){ process::task::add([=](){
            if(!cli.is_available() ){ cli.close(); return -1; }
            if((*_read)(&cli)==1 )   { return 1; } 
            if(  _read->c <= 0  )    { return 1; }
            cli.onData.emit(_read->y); return 1;
        }) ; });

        process::task::add([=](){
            server.onConnect.emit(cli); return -1;
        }); cli.onDrain([=](){ cli.free(); });

    }); return server; }

    /*─······································································─*/

    tls_t server( ssl_t* ctx, agent_t* opt=nullptr ){
        auto server = (tls_t){ [=]( ssocket_t cli ){}, ctx, opt };
        tls::server( server ); return server; 
    }

    /*─······································································─*/

    tls_t client( const tls_t& client ){ client.onOpen([=]( ssocket_t cli ){
        ptr_t<_file_::read> _read = new _file_::read;

        process::task::add([=](){
            if(!cli.is_available() ){ cli.close(); return -1; }
            if((*_read)(&cli)==1 )   { return 1; } 
            if(  _read->c <= 0  )    { return 1; }
            cli.onData.emit(_read->y); return 1;
        }); cli.onDrain([=](){ cli.free(); });

    }); return client; }

    /*─······································································─*/

    tls_t client( ssl_t* ctx, agent_t* opt=nullptr ){
        auto client = (tls_t){ [=]( ssocket_t cli ){}, ctx, opt };
        tls::client( client ); return client; 
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif

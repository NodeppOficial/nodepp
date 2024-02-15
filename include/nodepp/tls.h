#ifndef NODEPP_TLS
#define NODEPP_TLS

/*────────────────────────────────────────────────────────────────────────────*/

#include "ssocket.h"
#include "poll.h"
#include "dns.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

class tls_t {
protected:

    struct NODE {
        int                        state = 0;
        bool                       chck  = 1;
        agent_t                    agent;
        ssl_t                      ctx  ; 
        poll_t                     poll ;
        function_t<void,ssocket_t> func ;
    };  ptr_t<NODE> obj;
    
    /*─······································································─*/

    void init_poll_loop( ptr_t<const tls_t>& inp ) const noexcept { process::poll::add([=](){
        if( inp->is_closed() ){ return -1; } if( inp->obj->poll.emit() != -1 ) { auto x = inp->obj->poll.get_last_poll();
            if( x[0] == 0 ){ ssocket_t cli(inp->obj->ctx,x[1]); cli.set_sockopt(inp->obj->agent); inp->onSocket.emit(cli); inp->obj->func(cli); }
            if( x[0] == 1 ){ ssocket_t cli(inp->obj->ctx,x[1]); cli.set_sockopt(inp->obj->agent); inp->onSocket.emit(cli); inp->obj->func(cli); }
        #if _KERNEL == NODEPP_KERNEL_WINDOWS
            if( x[0] ==-1 ){ ::closesocket(x[1]); }
        #else
            if( x[0] ==-1 ){ ::close(x[1]); }
        #endif
        }   return 1; 
    }); }
    
public: tls_t() noexcept : obj( new NODE() ) {}

    event_t<ssocket_t> onConnect;
    event_t<ssocket_t> onSocket;
    event_t<>          onClose;
    event_t<except_t>  onError;
    event_t<ssocket_t> onOpen;
    
    /*─······································································─*/

    tls_t( decltype(NODE::func) _func, ssl_t* xtc, agent_t* opt=nullptr )
    : obj( new NODE() ){ 
    if( xtc == nullptr ) process::error("Invalid SSL Contenx");
        obj->agent = opt==nullptr ? agent_t():*opt; 
        obj->func = _func; obj->ctx = *xtc; 
    }

    /*─······································································─*/
    
    void     close() const noexcept { if( obj->state<0 ){ return; } obj->state=-1; onClose.emit(); }
    bool is_closed() const noexcept { return obj == nullptr ? 1 : obj->state < 0; }
    
    /*─······································································─*/

    void poll( bool chck ) const noexcept { obj->chck = chck; }
    
    /*─······································································─*/

    void listen( const string_t& host, int port, decltype(NODE::func)* cb=nullptr  ) const noexcept {
        if( obj->state == 1 ){ return; } obj->state = 1; if( obj->ctx.create_server() == -1 )
          { process::error(onError,"Error Initializing SSL context"); close(); return; }
        if( dns::lookup(host).empty() ){ process::error(onError,"dns couldn't get ip"); close(); return; }
            auto inp = type::bind( this );
        
        ssocket_t *sk = new ssocket_t; 
                   sk->PROT = IPPROTO_TCP;
                   sk->socket( dns::lookup(host), port );
        
        if( sk->bind()    < 0 ){ process::error(onError,"Error while binding TLS");   close(); delete sk; return; }
        if( sk->listen()  < 0 ){ process::error(onError,"Error while listening TLS"); close(); delete sk; return; }
        if( obj->chck == true ){ init_poll_loop( inp ); }

        onOpen.emit(*sk); if( cb != nullptr ){ (*cb)(*sk); } 
        
        process::task::add([=](){
            static int _accept = 0; 
        coStart

            while( sk != nullptr ){ _accept = sk->_accept();
                if( inp->is_closed() || !sk->is_available() )
                  { break; } elif ( _accept != -2 )
                  { break; } coYield(1);
            }
            
            if( _accept == -1 ){ process::error(inp->onError,"Error while accepting TLS"); coGoto(2); }
            elif ( !sk->is_available() || inp->is_closed() ){ coGoto(2); }
            elif ( inp->obj->chck == true ){ inp->obj->poll.push_read(_accept); coGoto(0); }
            else { ssocket_t cli( inp->obj->ctx, _accept ); if( cli.is_available() ){ 
                   process::poll::add([=]( ssocket_t cli ){
                        cli.set_sockopt( inp->obj->agent ); 
                        inp->onSocket.emit( cli ); 
                        inp->obj->func( cli ); 
                        return -1;
                   }, cli );
            } coGoto(0); } 

            coYield(2); inp->close(); delete sk; 
            
        coStop
        });

    }

    void listen( const string_t& host, int port, decltype(NODE::func) cb ) const noexcept { 
         listen( host, port, &cb ); 
    }
    
    /*─······································································─*/

    void connect( const string_t& host, int port, decltype(NODE::func)* cb=nullptr  ) const noexcept {
        if( obj->state == 1 ){ return; } obj->state = 1; if( obj->ctx.create_client() == -1 )
          { process::error(onError,"Error Initializing SSL context"); close(); return; }
        if( dns::lookup(host).empty() )
           { process::error(onError,"dns couldn't get ip"); close(); return; }
            auto inp = type::bind( this );

        ssocket_t sk = ssocket_t(); 
                  sk.PROT = IPPROTO_TCP;
                  sk.socket( dns::lookup(host), port );
                  sk.set_sockopt( obj->agent );

        if( sk.connect() < 0 ){ 
            process::error(onError,"Error while connecting TLS"); 
            close(); return; 
        }

        sk.ssl = new ssl_t( obj->ctx, sk.get_fd() ); 
        sk.ssl->set_hostname( host );

        if( sk.ssl->connect() <= 0 ){ 
            process::error(onError,"Error while handshaking TLS");
            close(); return; 
        }

        if( cb != nullptr ){ (*cb)(sk); } sk.onClose.on([=](){ inp->close(); });
        onOpen.emit(sk); sk.onOpen.emit(); onSocket.emit(sk); obj->func(sk); 
    }

    void connect( const string_t& host, int port, decltype(NODE::func) cb ) const noexcept { 
         connect( host, port, &cb ); 
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

namespace tls {
    
    tls_t server( const tls_t& server ){ server.onSocket([=]( ssocket_t cli ){
        ptr_t<_file_::read> _read = new _file_::read;
        cli.onDrain.once([=](){ cli.free(); });
        cli.busy();

        server.onConnect.once([=]( ssocket_t cli ){ process::poll::add([=](){
            if(!cli.is_available() ) { cli.close(); return -1; }
            if((*_read)(&cli)==1 )   { return 1; } 
            if(  _read->c  <=  0 )   { return 1; }
            cli.onData.emit(_read->y); return 1;
        }) ; });

        process::task::add([=](){
            server.onConnect.emit(cli); return -1;
        });

    }); server.poll( false ); return server; }

    /*─······································································─*/

    tls_t server( ssl_t* ctx, agent_t* opt=nullptr ){
        auto server = tls_t( [=]( ssocket_t /*unused*/ ){}, ctx, opt );
        tls::server( server ); return server; 
    }

    /*─······································································─*/

    tls_t client( const tls_t& client ){ client.onOpen.once([=]( ssocket_t cli ){
        ptr_t<_file_::read> _read = new _file_::read;
        cli.onDrain.once([=](){ cli.free(); });
        cli.busy();

        process::poll::add([=](){
            if(!cli.is_available() ) { cli.close(); return -1; }
            if((*_read)(&cli)==1 )   { return 1; } 
            if(  _read->c  <=  0 )   { return 1; }
            cli.onData.emit(_read->y); return 1;
        });

    }); return client; }

    /*─······································································─*/

    tls_t client( ssl_t* ctx, agent_t* opt=nullptr ){
        auto client = tls_t( [=]( ssocket_t /*unused*/ ){}, ctx, opt );
        tls::client( client ); return client; 
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif

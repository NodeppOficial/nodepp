#ifndef NODEPP_WS
#define NODEPP_WS
#ifndef SECRET
#define SECRET "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#include "http.h"
#include "crypto.h"
#include "generator.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class ws_t : public socket_t {
public:

    template< class... T > 
    ws_t( const T&... args ) : socket_t(args...) {}

    /*─······································································─*/

    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        int    x = socket_t::_read( bf, sx );
        return x<=0 ? x : read_ws_frame( bf, x );
    }
    
    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        int    x = write_ws_frame( bf, sx );
        return x<=0 ? x : socket_t::_write( bf, x );
    } 

}; }

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace ws {

    tcp_t server( const tcp_t& server ){ server.onSocket([=]( socket_t cli ){
        if ( !nodepp::WSServer( (http_t) cli ) ){ return; }
        ptr_t<_file_::read> _read = new _file_::read;
        cli.onDrain.once([=](){ cli.free(); });

        server.onConnect.once([=]( ws_t cli ){ cli.busy(); 
        process::poll::add([=](){
            if(!cli.is_available() ) { cli.close(); return -1; }
            if((*_read)(&cli)==1 )   { return 1; }
            if(  _read->c  <=  0 )   { return 1; }
            cli.onData.emit(_read->y); return 1;
        }) ; });

        process::task::add([=](){
            cli.resume(); server.onConnect.emit(cli); return -1;
        });
        
    }); return server; }

    /*─······································································─*/

    tcp_t server( agent_t* opt=nullptr ){
        auto server = http::server( [=]( http_t /*unused*/ ){}, opt );
                        ws::server( server ); return server; 
    }

    /*─······································································─*/

    ws_t client( const string_t& url, agent_t* opt=nullptr ){

        string_t hsh = crypto::genkey("abcdefghiABCDEFGHI0123456789",22);
        string_t key = string::format("%s==",hsh.data());
        ptr_t<_file_::read> _read = new _file_::read;

        fetch_t args;
                args.url = url;
                args.headers = {{
            { "Upgrade", "websocket" },
            { "Connection", "Upgrade" },
            { "Sec-Websocket-Key", key },
            { "Sec-Websocket-Version", "13" }
        }};

        ws_t cli = nodepp::WSClient( http::fetch( args, opt ), key );
             cli.onDrain.once([=](){ cli.free(); }); 

        cli.onOpen.once([=](){ cli.busy(); 
        process::poll::add([=](){
            if(!cli.is_available() ) { cli.close(); return -1; }
            if((*_read)(&cli)==1 )   { return 1; }
            if(  _read->c  <=  0 )   { return 1; }
            cli.onData.emit(_read->y); return 1;
        }) ; });

        process::task::add([=](){
            cli.resume(); cli.onOpen.emit(); return -1;
        });

        return cli;
    }

}}

#endif
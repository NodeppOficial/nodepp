#ifndef NODEPP_WSS
#define NODEPP_WSS
#ifndef SECRET
#define SECRET "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#include "https.h"
#include "crypto.h"
#include "generator.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class wss_t : public ssocket_t {
public:

    template< class... T > 
    wss_t( const T&... args ) noexcept : ssocket_t(args...) {}

    /*─······································································─*/
    
    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        int    x = ssocket_t::_read( bf, sx );
        return x<=0 ? x : read_ws_frame( bf, x );
    }
    
    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        int    x = write_ws_frame( bf, sx );
        return x<=0 ? x : ssocket_t::_write( bf, x );
    }

}; }

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace wss {

    tls_t server( const tls_t& server ){ server.onSocket([=]( ssocket_t cli ){
        if ( !nodepp::WSSServer( (https_t) cli ) ){ return; }
        ptr_t<_file_::read> _read = new _file_::read;
        cli.onDrain.once([=](){ cli.free(); });

        server.onConnect.once([=]( wss_t cli ){ cli.busy();
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

    tls_t server( ssl_t* ctx, agent_t* opt=nullptr ){
        auto server = https::server( [=]( https_t /*unused*/ ){}, ctx, opt );
                        wss::server( server ); return server;     
    }

    /*─······································································─*/

    wss_t client( const string_t& url, ssl_t* ctx, agent_t* opt=nullptr ){

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

        wss_t cli = nodepp::WSSClient( https::fetch( args, ctx, opt ), key );
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
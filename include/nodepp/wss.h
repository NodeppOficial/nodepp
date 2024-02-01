#ifndef NODEPP_WSS
#define NODEPP_WSS
#ifndef SECRET
#define SECRET "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#include "crypto.h"
#include "https.h"
#include "generator.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class wss_t : public ssocket_t {
public:

    template< class... T > wss_t( const T&... args ) noexcept : ssocket_t(args...) {}
    
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

    tls_t server( const tls_t& server ){ server.onSocket([=]( https_t cli ){
        nodepp::WSServer( cli, [=](){ ptr_t<_file_::read> _read = new _file_::read;

            server.onConnect([=]( wss_t cli ){ process::task::add([=](){ 
                if(!cli.is_available() ) { cli.close(); return -1; }
                if((*_read)(&cli)==1 )   { return 1; }
                if(  _read->c  <=  0 )   { return 1; }
                cli.onData.emit(_read->y); return 1;
            }) ; });

            process::task::add([=](){
                cli.resume(); server.onConnect.emit(cli); return -1;
            }); cli.onDrain([=](){ cli.free(); }); 

        });
    }); return server; }

    /*─······································································─*/

    tls_t server( ssl_t* ctx, agent_t* opt=nullptr ){
        auto server = https::server( [=]( ... ){}, ctx, opt );
                        wss::server( server ); return server;     
    }

    /*─······································································─*/

    wss_t client( const string_t& url, ssl_t* ctx, agent_t* opt=nullptr ){

        string_t hsh = hash::hash("abcdefghiABCDEFGHI0123456789",22);
        string_t key = string::format("%s==",hsh.data());

        fetch_t args;
                args.url = url;
                args.headers = {{
            { "Upgrade", "websocket" },
            { "Connection", "Upgrade" },
            { "Sec-Websocket-Key", key },
            { "Sec-Websocket-Version", "13" }
        }};

        auto req = https::fetch( args, ctx, opt );

        return nodepp::WSClient<wss_t>( req, key, [=]( wss_t cli ){
            ptr_t<_file_::read> _read = new _file_::read;

            cli.onOpen([=](){ process::task::add([=](){
                if(!cli.is_available() ) { cli.close(); return -1; }
                if((*_read)(&cli)==1 )   { return 1; }
                if(  _read->c  <=  0 )   { return 1; }
                cli.onData.emit(_read->y); return 1;
            }) ; });

            process::task::add([=](){
                cli.resume(); cli.onOpen.emit(); return -1;
            }); cli.onDrain([=](){ cli.free(); });
        
        });
         
    }

}}

#endif
#ifndef NODEPP_WSS
#define NODEPP_WSS
#define SECRET "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

/*────────────────────────────────────────────────────────────────────────────*/

#include "crypto.h"
#include "https.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace wss {

    tls_t server( const tls_t& server ){ server.onSocket([=]( https_t cli ){
        nodepp::WSServer( cli, [=](){ ptr_t<_file_::read> _read = new _file_::read;

            server.onConnect([=]( ssocket_t cli ){ process::task::add([=](){ 
                if(!cli.is_available() ){ cli.close(); return -1; }
                if((*_read)(&cli)==1 )   { return 1; } 
                if(  _read->c <= 0  )    { return 1; }
                cli.onData.emit(_read->y); return 1;
            });});

            process::task::add([=](){
                cli.resume(); server.onConnect.emit(cli); return -1;
            }); cli.onDrain([=](){ cli.free(); }); 

        });
    }); return server; }

    /*─······································································─*/

    tls_t server( ssl_t* ctx, agent_t* opt=nullptr ){
        auto server = https::server( [=]( https_t cli ){}, ctx, opt );
                        wss::server( server ); return server;     
    }

    /*─······································································─*/

    https_t client( const string_t& url, ssl_t* ctx, agent_t* opt=nullptr ){

        string_t key = hash::hash("abcdefABCDEF0123456789");

        auto cli = https::fetch({ .ssl=ctx, .agent=opt, .headers={{ 
            { "Sec-Websocket-Version", "13" },  
            { "Sec-Websocket-Key", key },   
            { "Connection", "Upgrade" },          
            { "Upgrade", "Websocket" }
        }}, .url = url });

        return nodepp::WSClient<https_t>( cli, key, [=]( ssocket_t cli ){
            ptr_t<_file_::read> _read = new _file_::read;

            cli.onOpen([=](){ process::task::add([=](){
                if(!cli.is_available() ){ cli.close(); return -1; }
                if((*_read)(&cli)==1 )   { return 1; } 
                if(  _read->c <= 0  )    { return 1; }
                cli.onData.emit(_read->y); return 1;
            });});

            process::task::add([=](){
                cli.resume(); cli.onOpen.emit(); return -1;
            }); cli.onDrain([=](){ cli.free(); });
        
        });
         
    }

}}

#endif
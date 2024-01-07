#ifndef NODEPP_WS
#define NODEPP_WS
#define SECRET "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

/*────────────────────────────────────────────────────────────────────────────*/

#include "crypto.h"
#include "http.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace ws {

    tcp_t server( const tcp_t& server ){ server.onSocket([=]( http_t cli ){
        nodepp::WSServer( cli, [=](){ ptr_t<_file_::read> _read = new _file_::read;

            server.onConnect([=]( socket_t cli ){ process::task::add([=](){
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

    tcp_t server( agent_t* opt=nullptr ){
        auto server = http::server( [=]( http_t cli ){}, opt );
                        ws::server( server ); return server; 
    }

    /*─······································································─*/

    http_t client( const string_t& url, agent_t* opt=nullptr ){

        string_t key = hash::hash("abcdefABCDEF0123456789");

        auto cli = http::fetch({ .agent=opt, .headers={{ 
            { "Sec-Websocket-Version", "13" },  
            { "Sec-Websocket-Key", key },   
            { "Connection", "Upgrade" },          
            { "Upgrade", "Websocket" }
        }}, .url = url });

        return nodepp::WSClient<http_t>( cli, key, [=]( socket_t cli ){
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
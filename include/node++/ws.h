#ifndef NODEPP_WS
#define NODEPP_WS
#define SECRET "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

/*────────────────────────────────────────────────────────────────────────────*/

#include "crypto.h"
#include "fetch.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace { template< class T, class U > void WSServer( T cli, U cb ) {
    
    auto data = cli.read(); cli.set_borrow( data ); int c=0;
    
    while(( c=cli.read_header() )>0 ){ process::next(); }
       if( c == -1 ){ return; }

    if( !cli.headers["Sec-Websocket-Key"].empty() ){

        string_t sec = regex::match(cli.headers["Sec-Websocket-Key"],"[^\\s\n ]+");
        string_t key = sec + SECRET;

            auto sha = crypto::SHA1();         sha.update(key);
            auto b64 = crypto::enc::BASE64();  b64.update(sha.done());
            auto enc = b64.done().slice(0,-1);

        cli.write_headers( 101, {{
            { "Sec-Websocket-Accept", enc },
            { "Connection", "Upgrade" },
            { "Upgrade", "Websocket" }
        }});
        
        cb(); cli.stop(); return;
    }   cli.set_borrow( data );

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace { template< class U, class T, class V > U WSClient( T fetch, string_t key, V cb ) {
    auto res = fetch.await(); if( tuple::get<0>(res) == 1 ) _Error( tuple::get<2>(res).what() );
    auto cli = tuple::get<1>(res);

    if(  cli.status != 101 ){ _onError(cli.onError,"WSE: Can't connect to WS Server"); }
    if( !cli.headers["Sec-Websocket-Accept"].empty() ){

        string_t dta = regex::match(cli.headers["Sec-Websocket-Accept"],"[^\\s\n ]+");
        string_t sec = key + SECRET;

            auto sha = crypto::SHA1();         sha.update(sec);
            auto b64 = crypto::enc::BASE64();  b64.update(sha.done());
            auto enc = b64.done().slice(0,-1);

    if( dta != enc ){ _onError(cli.onError,"WSE: secret key does not match"); } 
        cb(cli); cli.stop();
    }   return cli;

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace ws {

    tcp_t server( const tcp_t& server ){ server.onSocket([=]( http_t cli ){
        nodepp::WSServer( cli, [=](){ 
            ptr_t<_file_::read> _read = new _file_::read;

            server.onConnect([=]( socket_t cli ){ process::task::add([=](){
                while(!cli.is_available() ){ cli.close(); return -1; }
                while((*_read)(&cli)==1 ){ return 1; } 
                   if( _read->c<=0 )     { return 1; }
                cli.onData.emit(_read->y); return 1;
            }) ; });

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

    http_t client( string_t url, agent_t* opt=nullptr ){

        string_t key = hash::hash("abcdefABCDEF0123456789");

        auto cli = fetch::http({ .agent=opt, .headers={{ 
            { "Sec-Websocket-Version", "13" },  
            { "Sec-Websocket-Key", key },   
            { "Connection", "Upgrade" },          
            { "Upgrade", "Websocket" }
        }}, .url = url });

        return nodepp::WSClient<http_t>( cli, key, [=]( socket_t cli ){
            ptr_t<_file_::read> _read = new _file_::read;

            server.onOpen([=]( socket_t cli ){ process::task::add([=](){
                while(!cli.is_available() ){ cli.close(); return -1; }
                while((*_read)(&cli)==1 ){ return 1; } 
                   if( _read->c<=0 )     { return 1; }
                cli.onData.emit(_read->y); return 1;
            });});

            process::task::add([=](){
                cli.resume(); cli.onOpen.emit(); return -1;
            }); cli.onDrain([=](){ cli.free(); });
        });
         
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace wss {

    tls_t server( const tls_t& server ){ server.onSocket([=]( https_t cli ){
        nodepp::WSServer( cli, [=](){
            ptr_t<_file_::read> _read = new _file_::read;

            server.onConnect([=]( ssocket_t cli ){ process::task::add([=](){ 
                while(!cli.is_available() ){ cli.close(); return -1; }
                while((*_read)(&cli)==1 ){ return 1; } 
                   if( _read->c<=0 )     { return 1; }
                cli.onData.emit(_read->y); return 1;
            }) ; });

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

    https_t client( string_t url, ssl_t* ctx, agent_t* opt=nullptr ){

        string_t key = hash::hash("abcdefABCDEF0123456789");

        auto cli = fetch::https({ .ssl=ctx, .agent=opt, .headers={{ 
            { "Sec-Websocket-Version", "13" },  
            { "Sec-Websocket-Key", key },   
            { "Connection", "Upgrade" },          
            { "Upgrade", "Websocket" }
        }}, .url = url });

        return nodepp::WSClient<https_t>( cli, key, [=]( ssocket_t cli ){
            ptr_t<_file_::read> _read = new _file_::read;

            server.onOpen([=]( ssocket_t cli ){ process::task::add([=](){
                while(!cli.is_available() ){ cli.close(); return -1; }
                while((*_read)(&cli)==1 ){ return 1; } 
                   if( _read->c<=0 )     { return 1; }
                cli.onData.emit(_read->y); return 1;
            });});

            process::task::add([=](){
                cli.resume(); cli.onOpen.emit(); return -1;
            }); cli.onDrain([=](){ cli.free(); });
        
        });
         
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
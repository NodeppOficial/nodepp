/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_WSS
#define NODEPP_WSS
#define SECRET "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

/*────────────────────────────────────────────────────────────────────────────*/

#include "https.h"
#include "crypto.h"
#include "generator.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class wss_t : public ssocket_t {
public:

    ptr_t<_ws_::write> _write_ = new _ws_::write();
    ptr_t<_ws_::read>  _read_  = new _ws_::read();

    template< class... T > 
    wss_t( const T&... args ) noexcept : ssocket_t(args...) {}

    /*─······································································─*/
    
    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        while((*_read_)( bf, sx )==-1 && is_available() && _read_->state>0 ){
        while((_read_->input=ssocket_t::_read( bf, _read_->size ))==-2 )
              { return -2; } if( _read_->input<=0 ){ _read_->output=0; }
        }       obj->feof=_read_->output; return _read_->output;
    }
  
    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        while((*_write_)( bf, sx )==-1 && is_available() && _write_->state>0 ){
        while((_write_->input=ssocket_t::_write( bf, _write_->size ))==-2 )
              { return -2; } if( _write_->input<=0 ){ _write_->output=0; }
        }       obj->feof=_write_->output; return _write_->output;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace wss {

    tls_t server( const tls_t& server ){ server.onSocket([=]( ssocket_t cli ){
        if ( !nodepp::WSSServer( (https_t) cli ) ){ return; }
        ptr_t<_file_::read> _read = new _file_::read;
        cli.onDrain.once([=](){ cli.free(); });
        cli.set_timeout(0);

        server.onConnect.once([=]( wss_t cli ){
        process::poll::add([=](){ 
            if(!cli.is_available() )    { cli.close(); return -1; }
            if((*_read)(&cli)==1 )      { return 1; }
            if(  _read->state<=0 )      { return 1; }
            cli.onData.emit(_read->data); return 1;
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
              cli.set_timeout(0);

        cli.onOpen.once([=](){
        process::poll::add([=](){
            if(!cli.is_available() )    { cli.close(); return -1; }
            if((*_read)(&cli)==1 )      { return 1; }
            if(  _read->state<=0 )      { return 1; }
            cli.onData.emit(_read->data); return 1;
        }) ; });

        process::task::add([=](){
            cli.resume(); cli.onOpen.emit(); return -1;
        });

        return cli; 
    }

}}

#undef SECRET
#endif
/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_WS
#define NODEPP_WS
#define SECRET "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

/*────────────────────────────────────────────────────────────────────────────*/

#include "http.h"
#include "crypto.h"
#include "generator.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class ws_t : public socket_t {
protected:

    ptr_t<_ws_::write> _write_ = new _ws_::write();
    ptr_t<_ws_::read>  _read_  = new _ws_::read();

public:

    template< class... T > 
    ws_t( const T&... args ) noexcept : socket_t(args...) {}

    /*─······································································─*/
    
    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        while((*_read_)( bf, sx )==-1 && is_available() && _read_->state>0 ){
        while((_read_->input=socket_t::_read( bf, _read_->size )) ==-2 )
              { return -2; } if( _read_->input<=0 ){ _read_->output=-1; }
        }       obj->feof=_read_->output; return _read_->output;
    }
  
    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        while((*_write_)( bf, sx )==-1 && is_available() && _write_->state>0 ){
        while((_write_->input=socket_t::_write( bf, _write_->size ))==-2 )
              { return -2; } if( _write_->input<=0 ){ _write_->output=-1; }
        }       obj->feof=_write_->output; return _write_->output;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace ws {

    tcp_t server( const tcp_t& srv ){ srv.onSocket([=]( socket_t cli ){
        
        ptr_t<_file_::read> _read = new _file_::read;
        auto hrv = type::cast<http_t>( cli );
        auto ctx = type::cast<ws_t>( cli );

        if ( !_ws_::server( hrv ) ){ return; }
        cli.onDrain.once([=](){ cli.free(); });
        cli.set_timeout(0);

        srv.onConnect.once([=]( ws_t ctx ){ process::poll::add([=](){
            if(!cli.is_available() )    { cli.close(); return -1; }
            if((*_read)(&ctx)==1 )      { return 1; }
            if(  _read->state<=0 )      { return 1; }
            cli.onData.emit(_read->data); return 1;
        }); });

        process::task::add([=](){
            cli.resume(); srv.onConnect.emit(ctx); return -1;
        });
        
    }); return srv; }

    /*─······································································─*/

    tcp_t server( agent_t* opt=nullptr ){
        auto server = http::server( [=]( http_t /*unused*/ ){}, opt );
                        ws::server( server ); return server; 
    }

    /*─······································································─*/

    tcp_t client( const string_t& uri, agent_t* opt=nullptr ){ 
    tcp_t srv ( [=]( socket_t /*unused*/ ){}, opt ); 
        srv.connect( url::hostname(uri), url::port(uri) );
        srv.onSocket.once([=]( socket_t cli ){

            ptr_t<_file_::read> _read = new _file_::read;
            auto hrv = type::cast<http_t>( cli );
            auto ctx = type::cast<ws_t>( cli );

            if ( !_ws_::client( hrv, uri ) ){ return; }
            cli.onDrain.once([=](){ cli.free(); }); 
            cli.set_timeout(0);

            srv.onConnect.once([=]( ws_t ctx ){ process::poll::add([=](){
                if(!cli.is_available() )    { cli.close(); return -1; }
                if((*_read)(&ctx)==1 )      { return 1; }
                if(  _read->state<=0 )      { return 1; }
                cli.onData.emit(_read->data); return 1;
            }); });

            process::task::add([=](){
                cli.resume(); srv.onConnect.emit(ctx); return -1;
            });
        
        });
    
    return srv; }

}}

#undef SECRET
#endif
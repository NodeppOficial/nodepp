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
public:

    template< class... T > 
    ws_t( const T&... args ) noexcept : socket_t( args... ), 
    _write_( new _ws_::write() ), _read_( new _ws_::read() ) {}

    /*─······································································─*/
    
    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        while((*_read_)( bf, sx )==-1 && is_available() && _read_->state ){
        while(( _read_->input=__read( bf, _read_->size ))==-2 ){ return -2; }
        }       obj->feof=_read_->output; return _read_->output;
    }
  
    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        while((*_write_)( bf, sx )==-1 && is_available() && _write_->state ){
        while(( _write_->input=__write( bf, _write_->size ))==-2 ){ return -2; }
        }       obj->feof=_write_->output; return _write_->output;
    }

protected:

    int __write( char* bf, const ulong& sx ) const noexcept {
        return socket_t::_write( bf, sx );
    }

    int __read( char* bf, const ulong& sx ) const noexcept {
        return socket_t::_read( bf, sx );
    }

    /*─······································································─*/

    ptr_t<_ws_::write> _write_;
    ptr_t<_ws_::read>  _read_ ;

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace ws {

    tcp_t server( const tcp_t& srv ){ srv.onSocket([=]( socket_t cli ){

        cli.onDrain.once([=](){ cli.free(); cli.onData.clear(); });
        ptr_t<_file_::read> _read = new _file_::read;
        cli.set_timeout(0);
        
        auto hrv = type::cast<http_t>( cli );
        if ( !_ws_::server( hrv ) ){ return; }

        srv.onConnect.once([=]( ws_t ctx ){ process::poll::add([=](){
            if(!cli.is_available() )    { cli.close(); return -1; }
            if((*_read)(&ctx)==1 )      { return 1; }
            if(  _read->state<=0 )      { return 1; }
            ctx.onData.emit(_read->data); return 1;
        }); });

        process::task::add([=](){
            cli.resume(); srv.onConnect.emit(cli); return -1;
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

            cli.onDrain.once([=](){ cli.free(); cli.onData.clear(); }); 
            ptr_t<_file_::read> _read = new _file_::read;
            cli.set_timeout(0);

            auto hrv = type::cast<http_t>( cli );
            if ( !_ws_::client( hrv, uri ) ){ return; }

            srv.onConnect.once([=]( ws_t ctx ){ process::poll::add([=](){
                if(!cli.is_available() )    { cli.close(); return -1; }
                if((*_read)(&ctx)==1 )      { return 1; }
                if(  _read->state<=0 )      { return 1; }
                ctx.onData.emit(_read->data); return 1;
            }); });

            process::task::add([=](){
                cli.resume(); srv.onConnect.emit(cli); return -1;
            });
        
        });
    
    return srv; }

}}

#undef SECRET
#endif
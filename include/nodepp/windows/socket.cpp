/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.nodepp.xyz/license.html
 */

/*────────────────────────────────────────────────────────────────────────────*/

#pragma once

#include <winsock.h>
#include <ws2tcpip.h>
#include <winsock2.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace _socket_ {

    void start_device(){ static bool sockets=false; 
        if( sockets == false ){ WSADATA wsaData;

            process::onSIGEXIT([=](){
                #ifdef SIGPIPE
                    process::signal::unignore( SIGPIPE );
                #endif 
                    WSACleanup();
            });

                #ifdef SIGPIPE
                    process::signal::ignore( SIGPIPE );
                #endif
            
            if( WSAStartup(MAKEWORD(2,2),&wsaData) != 0 ){
                process::error("Failed to initialize Winsock");
            }
        }   sockets = true;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

struct agent_t {
    bool  reuse_address = 1;
    uint  recv_timeout  = 0;
    uint  send_timeout  = 0;
    ulong buffer_size   = CHUNK_SIZE;
    bool  reuse_port    = 1;
    bool  keep_alive    = 0;
    bool  broadcast     = 0;
    bool  reconnect     = 10;
};

class socket_t {
protected:

    using TIMEVAL     = struct timeval;
    using SOCKADDR    = struct sockaddr;
    using SOCKADDR_IN = struct sockaddr_in;

    struct NODE {

        SOCKADDR server_addr, client_addr;
        int addrlen; bool srv=0; int len;
        int _retry=10, retry=10;
        int feof = 1;

        SOCKET       fd = INVALID_SOCKET;
        ulong        range[2] = { 0, 0 };
        int          state = 0;
        ptr_t<char>  buffer;
        string_t     borrow;

    };  ptr_t<NODE> obj = new NODE();
    
    /*─······································································─*/

    virtual bool is_blocked( int& c ) const noexcept {
        if ( c >= 0 ){ return 0; } auto error =  WSAGetLastError(); 
        if ( error == WSAEISCONN ){ c=0; return 0; } 
      elif ( error == WSAECONNRESET ){
        if ( obj->retry<=0 )             return 0;
             obj->retry--;               return 1;
        }    obj->retry = obj->_retry;   return (
             error == WSAEWOULDBLOCK || error == WSAEINPROGRESS ||
             error == WSAEALREADY
        );
    }
    
    /*─······································································─*/
    
    virtual int set_nonbloking_mode() const noexcept { ulong mode = 1;
        return ioctlsocket( obj->fd, FIONBIO, &mode ) != 0 ? -1 : 1;
    }

public: socket_t() noexcept { _socket_::start_device(); }

    event_t<>          onUnpipe;
    event_t<>          onResume;
    event_t<except_t>  onError;
    event_t<>          onDrain;
    event_t<>          onClose;
    event_t<>          onStop;
    event_t<>          onOpen;
    event_t<>          onPipe;
    event_t<string_t>  onData;
    
    /*─······································································─*/

    int SOCK  = SOCK_STREAM;
    int AF    = AF_INET; 
    int PROT  = 0;
    
    /*─······································································─*/

    int set_recv_buff( ulong en ) const noexcept { int c;
        while( is_blocked( c=setsockopt( obj->fd, SOL_SOCKET, SO_RCVBUF, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_send_buff( ulong en ) const noexcept { int c;
        while( is_blocked( c=setsockopt( obj->fd, SOL_SOCKET, SO_SNDBUF, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_recv_timeout( long time ) const noexcept { int c; TIMEVAL en; en.tv_sec=time; 
        while( is_blocked( c=setsockopt( obj->fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c; 
    }

    int set_send_timeout( long time ) const noexcept { int c; TIMEVAL en; en.tv_sec=time;
        while( is_blocked( c=setsockopt( obj->fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_accept_connection( ulong en ) const noexcept { int c;
        while( is_blocked( c=setsockopt( obj->fd, SOL_SOCKET, SO_ACCEPTCONN, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_dont_route( ulong en ) const noexcept { int c;
        while( is_blocked( c=setsockopt( obj->fd, SOL_SOCKET, SO_DONTROUTE, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_keep_alive( uint en ) const noexcept { int c;
        while( is_blocked( c=setsockopt( obj->fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_broadcast( uint en ) const noexcept { int c;
        while( is_blocked( c=setsockopt( obj->fd, SOL_SOCKET, SO_BROADCAST, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_reuse_address( uint en ) const noexcept { int c;
        while( is_blocked( c=setsockopt( obj->fd, SOL_SOCKET, SO_REUSEADDR, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

#ifdef SO_REUSEPORT
    int set_reuse_port( uint en ) const noexcept { int c;
        while( is_blocked( c=setsockopt( obj->fd, SOL_SOCKET, SO_REUSEPORT, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }
#endif

    int set_reconnect( uint en ) const noexcept { obj->_retry = en; obj->retry = en; return en; }

    /*─······································································─*/

    int get_error() const noexcept { int c, en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_ERROR, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en : c;
    }

    int get_recv_buff() const noexcept { int c, en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_RCVBUF, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en : c;
    }

    int get_send_buff() const noexcept { int c, en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_SNDBUF, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en : c;
    }

    long get_recv_timeout() const noexcept { int c; TIMEVAL en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en.tv_sec : c;
    }

    long get_send_timeout() const noexcept { int c; TIMEVAL en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en.tv_sec : c;
    }

    int get_accept_connection() const noexcept { int c, en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_ACCEPTCONN, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en : c;
    }

    int get_dont_route() const noexcept { int c, en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_DONTROUTE, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en : c;
    }

    int get_reuse_address() const noexcept { int c, en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_REUSEADDR, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en : c;
    }

#ifdef SO_REUSEPORT
    int get_reuse_port() const noexcept { int c, en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_REUSEPORT, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en : c;
    }
#endif

    int get_keep_alive() const noexcept { int c, en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en : c;
    }

    int get_broadcast() const noexcept { int c, en; socklen_t size = sizeof(en);
        while( is_blocked( c=getsockopt(obj->fd, SOL_SOCKET, SO_BROADCAST, (char*)&en, &size) ) )
             { process::next(); } return c==0 ? en : c;
    }

    int get_reconnect() const noexcept { return obj->_retry; }
    
    /*─······································································─*/

    string_t get_sockname() const noexcept { int c; string_t buff { INET_ADDRSTRLEN };
        SOCKADDR cli; if( obj->srv==1 ) cli = obj->client_addr; else cli = obj->server_addr;
        while( is_blocked( c=getsockname( obj->fd, &cli, &obj->len )) ){ process::next(); }
        inet_ntop( AF, &(((SOCKADDR_IN*)&cli)->sin_addr), (char*)buff, buff.size() );
        return c < 0 ? "127.0.0.1" : buff;
    }

    string_t get_peername() const noexcept { int c; string_t buff { INET_ADDRSTRLEN };
        SOCKADDR cli; if( obj->srv==1 ) cli = obj->client_addr; else cli = obj->server_addr;
        while( is_blocked( c=getpeername( obj->fd, &cli, &obj->len )) ){ process::next(); }
        inet_ntop( AF, &(((SOCKADDR_IN*)&cli)->sin_addr), (char*)buff, buff.size() );
        return c < 0 ? "127.0.0.1" : buff;
    }
    
    /*─······································································─*/

    virtual ulong set_buffer_size( ulong _size ) const noexcept { 
        set_send_buff( _size ); set_recv_buff( _size );
        obj->buffer = ptr_t<char>(_size); return _size;
    }
    
    /*─······································································─*/

    int set_timeout( long time ) const noexcept {
        if( set_recv_timeout( time )<0 ){ return -1; } 
        if( set_send_timeout( time )<0 ){ return -1; } return 1; 
    }

    /*─······································································─*/

            bool    is_closed() const noexcept { return obj->state <  0 ||  is_feof() || obj->fd == INVALID_SOCKET; }
            bool is_available() const noexcept { return obj->state >= 0 && !is_closed(); }
    virtual bool      is_feof() const noexcept { return obj->feof  == 0; }
            bool    is_server() const noexcept { return obj->srv; }

    /*─······································································─*/
    
    void resume() const noexcept { if(obj->state== 0) { return; } obj->state= 0; onResume.emit(); }
    void  close() const noexcept { if(obj->state < 0) { return; } obj->state=-1; onDrain.emit();  }
    void   stop() const noexcept { if(obj->state==-3) { return; } obj->state=-3; onStop.emit();   }
    void  reset() const noexcept { if(obj->state!=-2) { return; } resume(); pos(0); }
    void  flush() const noexcept { obj->buffer.fill(0); }
    void   free() const noexcept { force_close(); } 
    
    /*─······································································─*/

    SOCKET    get_fd() const noexcept { return obj == nullptr ? INVALID_SOCKET : obj->fd;    }
    ulong* get_range() const noexcept { return obj == nullptr ?        nullptr : obj->range; }
    int    get_state() const noexcept { return obj == nullptr ?             -1 : obj->state; }
    
    /*─······································································─*/

    void   set_borrow( const string_t& brr ) const noexcept { obj->borrow = brr; }
    ulong  get_borrow_size() const noexcept { return obj->borrow.size(); }
    char*  get_borrow_data() const noexcept { return obj->borrow.data(); }
    void        del_borrow() const noexcept { obj->borrow.clear(); }
    string_t&   get_borrow() const noexcept { return obj->borrow; }
    
    /*─······································································─*/

    ulong   get_buffer_size() const noexcept { return obj->buffer.size(); }
    char*   get_buffer_data() const noexcept { return obj->buffer.data(); }
    ptr_t<char>& get_buffer() const noexcept { return obj->buffer; }
    
    /*─······································································─*/
    
    ulong pos( ulong /*unused*/ ) const noexcept { return 0; }

    ulong size() const noexcept { return 0; }

    ulong  pos() const noexcept { return 0; }
    
    /*─······································································─*/

    void set_sockopt( agent_t opt ) const noexcept { 
        set_reuse_address( opt.reuse_address );
        set_recv_timeout ( opt.recv_timeout  );
        set_send_timeout ( opt.send_timeout  );
        set_buffer_size  ( opt.buffer_size   );
    #ifdef SO_REUSEPORT
        set_reuse_port   ( opt.reuse_port    );
    #endif
        set_keep_alive   ( opt.keep_alive    );
        set_broadcast    ( opt.broadcast     );
        set_reconnect    ( opt.reconnect     );
    }

    agent_t get_sockopt() const noexcept { 
    agent_t opt;
        opt.reuse_address = get_reuse_address();
        opt.recv_timeout  = get_recv_timeout();
        opt.send_timeout  = get_send_timeout();
        opt.buffer_size   = get_buffer_size();
    #ifdef SO_REUSEPORT
        opt.reuse_port    = get_reuse_port();
    #endif
        opt.keep_alive    = get_keep_alive();
        opt.broadcast     = get_broadcast();
        opt.reconnect     = get_reconnect();
    return opt;
    }
    
    /*─······································································─*/
    
    virtual ~socket_t() noexcept {
        if( obj.count() > 1 ){ return; } 
        if( obj->state ==-2 ){ return; } force_close();
    }
    
    /*─······································································─*/

    socket_t( SOCKET fd, ulong _size=CHUNK_SIZE ){ _socket_::start_device();
        if( fd == INVALID_SOCKET ) process::error("Such Socket has an Invalid fd");
            obj->fd = fd; set_nonbloking_mode(); set_buffer_size(_size);
    }

    /*─······································································─*/

    virtual void force_close() const noexcept {
        if( obj->state == -3 && obj.count() > 1 ){ resume(); return; }
        if( obj->state == -2 ){ return; } obj->state = -2;
        ::shutdown(obj->fd,SD_BOTH); closesocket(obj->fd); 
        close(); onClose.emit();
    }

    /*─······································································─*/

    virtual int socket( const string_t& host, int port ) noexcept {
        if( host.empty() )
          { process::error(onError,"dns coudn't found ip"); return -1; }
        
        obj->addrlen = sizeof( obj->server_addr ); _socket_::start_device(); 

        if((obj->fd=::socket( AF, SOCK, PROT )) == INVALID_SOCKET )
          { process::error(onError,"can't initializate socket fd"); return -1; } 
          
        set_buffer_size( CHUNK_SIZE );
        set_nonbloking_mode();
        set_reuse_address(1);

    #ifdef SO_REUSEPORT
        set_reuse_port(1);
    #endif

        SOCKADDR_IN server, client;
        memset(&server, 0, sizeof(SOCKADDR_IN));
        memset(&client, 0, sizeof(SOCKADDR_IN));
        
        server.sin_family  = AF; if( port>0 ) 
        server.sin_port    = htons(port);

          if( host == "0.0.0.0"         || host == "globalhost" ){ server.sin_addr.s_addr = INADDR_ANY; }
        elif( host == "1.1.1.1"         || host == "loopback" )  { server.sin_addr.s_addr = INADDR_LOOPBACK; }
        elif( host == "255.255.255.255" || host == "broadcast" ) { server.sin_addr.s_addr = INADDR_BROADCAST; } 
        elif( host == "127.0.0.1"       || host == "localhost" ) { inet_pton(AF, "127.0.0.1", &server.sin_addr); }
        else                                                     { inet_pton(AF, host.c_str(),&server.sin_addr); }

        obj->server_addr = *((SOCKADDR*) &server);
        obj->client_addr = *((SOCKADDR*) &client);
        obj->len = sizeof( server ); return 1;
    }
    
    /*─······································································─*/

    int _connect() const noexcept { int c=0; if( obj->srv == 1 ){ return -1; }
        return is_blocked( c=::connect( obj->fd, &obj->server_addr, obj->addrlen ) ) ? -2 : c;
    }

    int _accept() const noexcept { int c=0; if( obj->srv == 0 ){ return -1; }
        return is_blocked( c=::accept( obj->fd, &obj->server_addr, &obj->addrlen ) ) ? -2 : c;
    }

    int _bind() const noexcept { int c=0; obj->srv = 1;
        return is_blocked( c=::bind( obj->fd, &obj->server_addr, obj->addrlen ) ) ? -2 : c;
    }

    int _listen() const noexcept { 
        int c=0; if( obj->srv == 0 ){ return -1; }
        return is_blocked( c=::listen( obj->fd, MAX_SOCKET ) ) ? -2 : c;
    }

    /*─······································································─*/

    int connect() const noexcept { int c=0;
        while( (c=_connect())==-2 ){ process::next(); } return c;
    }

    int listen() const noexcept { int c=0;
        while( (c=_listen())==-2 ){ process::next(); } return c;
    }

    int accept() const noexcept { int c=0;
        while( (c=_accept())==-2 ){ process::next(); } return c;
    }

    int bind() const noexcept { int c=0; 
        while( (c=_bind())==-2 ){ process::next(); } return c;
    }
    
    /*─······································································─*/

    char read_char() const noexcept { return read(1)[0]; }
    
    string_t read_line() const noexcept {
        auto gen = nodepp::_file_::line();
        while( gen( this ) == 1 )
             { process::next(); }
        return gen.data;
    }
    
    /*─······································································─*/

    string_t read( ulong size=CHUNK_SIZE ) const noexcept {
        auto gen = nodepp::_file_::read();
        while( gen( this, size ) == 1 )
             { process::next(); }
        return gen.data;
    }

    ulong write( const string_t& msg ) const noexcept {
        auto gen = nodepp::_file_::write();
        while( gen( this, msg ) == 1 )
             { process::next(); }
        return gen.data;
    }
    
    /*─······································································─*/

    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } if( sx==0 ){ return 0; } if( SOCK != SOCK_DGRAM ){
            obj->feof = ::recv( obj->fd, bf, sx, 0 ); return is_blocked(obj->feof) ? -2 : obj->feof;
        } else { SOCKADDR* cli; if( obj->srv==1 ) cli = &obj->client_addr; else cli = &obj->server_addr;
            obj->feof = ::recvfrom( obj->fd, bf, sx, 0, cli, &obj->len ); return is_blocked(obj->feof) ? -2 : obj->feof;
        }   return -1;
    }
    
    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } if( sx==0 ){ return 0; } if( SOCK != SOCK_DGRAM ){
            obj->feof = ::send( obj->fd, bf, sx, 0 ); return is_blocked(obj->feof) ? -2 : obj->feof;
        } else { SOCKADDR* cli; if( obj->srv==1 ) cli = &obj->client_addr; else cli = &obj->server_addr;
            obj->feof = ::sendto( obj->fd, bf, sx, 0, cli, obj->len ); return is_blocked(obj->feof) ? -2 : obj->feof;
        }   return -1;
    } 
    
};}

/*────────────────────────────────────────────────────────────────────────────*/
#ifndef NODEPP_SOCKET
#define NODEPP_SOCKET

/*────────────────────────────────────────────────────────────────────────────*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netdb.h>

#include "stream.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

struct agent_t {
    bool  reuse_address =  1;
    uint  recv_timeout  = 60;
    uint  send_timeout  = 60;
    ulong buffer_size   = CHUNK_SIZE;
    bool  reuse_port    = 1;
    bool  keep_alive    = 0;
    bool  broadcast     = 0;
};

namespace _socket_ { struct str {
    socklen_t addrlen; bool srv=0; socklen_t len;
    struct sockaddr server_addr, client_addr;
};}

/*────────────────────────────────────────────────────────────────────────────*/

class socket_t : public file_t {
protected:

    using TIMEVAL     = struct timeval;
    using SOCKADDR    = struct sockaddr;
    using SOCKADDR_IN = struct sockaddr_in;

    ptr_t<_socket_::str> skt = new _socket_::str();

public:

    int SOCK  = SOCK_STREAM;
    int AF    = AF_INET; 
    int PROT  = 0;
    
    /*─······································································─*/

    int set_recv_buff( ulong en ) const noexcept { int c;
        while( is_blocked( c=::setsockopt( obj->fd, SOL_SOCKET, SO_RCVBUF, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_send_buff( ulong en ) const noexcept { int c;
        while( is_blocked( c=::setsockopt( obj->fd, SOL_SOCKET, SO_SNDBUF, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_recv_timeout( long time ) const noexcept { int c; TIMEVAL en; en.tv_sec=time; 
        while( is_blocked( c=::setsockopt( obj->fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c; 
    }

    int set_send_timeout( long time ) const noexcept { int c; TIMEVAL en; en.tv_sec=time;
        while( is_blocked( c=::setsockopt( obj->fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_accept_connection( ulong en ) const noexcept { int c;
        while( is_blocked( c=::setsockopt( obj->fd, SOL_SOCKET, SO_ACCEPTCONN, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_dont_route( ulong en ) const noexcept { int c;
        while( is_blocked( c=::setsockopt( obj->fd, SOL_SOCKET, SO_DONTROUTE, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_keep_alive( uint en ) const noexcept { int c;
        while( is_blocked( c=::setsockopt( obj->fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_broadcast( uint en ) const noexcept { int c;
        while( is_blocked( c=::setsockopt( obj->fd, SOL_SOCKET, SO_BROADCAST, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

    int set_reuse_address( uint en ) const noexcept { int c;
        while( is_blocked( c=::setsockopt( obj->fd, SOL_SOCKET, SO_REUSEADDR, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }

#ifdef linux
    int set_reuse_port( uint en ) const noexcept { int c;
        while( is_blocked( c=::setsockopt( obj->fd, SOL_SOCKET, SO_REUSEPORT, (char*)&en, sizeof(en) ) ) )
             { process::next(); } return c;
    }
#endif

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

#ifdef linux
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
    
    /*─······································································─*/

    string_t get_ip() const noexcept { int c; string_t buff { INET_ADDRSTRLEN };
        SOCKADDR cli; if( skt->srv==1 ) cli = skt->client_addr; else cli = skt->server_addr;
        while( is_blocked( c=::getpeername( obj->fd, &cli, &skt->len )) ){ process::next(); }
        inet_ntop( AF, &(((SOCKADDR_IN*)&cli)->sin_addr), (char*)buff, buff.size() );
        return c<0 ? "127.0.0.1" : buff;
    }
    
    /*─······································································─*/

    virtual ulong set_buffer_size( ulong _size ) noexcept { 
        set_send_buff( _size ); set_recv_buff( _size );
        obj->buffer = ptr_t<char>(_size); return _size;
    }
    
    /*─······································································─*/

    int set_timeout( long time ) const noexcept {
        if( set_recv_timeout( time )<0 ){ return -1; } 
        if( set_send_timeout( time )<0 ){ return -1; } return 1; 
    }
    
    /*─······································································─*/

    virtual bool   is_feof() const noexcept { return get_error() != 0; }
            bool is_server() const noexcept { return skt->srv; }
    
    /*─······································································─*/

    int set_sockopt( agent_t* opt ) noexcept { 
    if( !opt ){ return -1; }
        set_reuse_address( opt->reuse_address );
        set_recv_timeout ( opt->recv_timeout  );
        set_send_timeout ( opt->send_timeout  );
        set_buffer_size  ( opt->buffer_size   );
    #ifdef linux
        set_reuse_port   ( opt->reuse_port    );
    #endif
        set_keep_alive   ( opt->keep_alive    );
        set_broadcast    ( opt->broadcast     );
    return 1;
    }

    agent_t* get_sockopt() const noexcept { 
    agent_t* opt;
        opt->reuse_address = get_reuse_address();
        opt->recv_timeout  = get_recv_timeout();
        opt->send_timeout  = get_send_timeout();
        opt->buffer_size   = get_buffer_size();
    #ifdef linux
        opt->reuse_port    = get_reuse_port();
    #endif
        opt->keep_alive    = get_keep_alive();
        opt->broadcast     = get_broadcast();
    return opt;
    }
    
    /*─······································································─*/
    
    virtual ~socket_t() noexcept {
        if( obj.count() > 1 || obj->fd < 3 ){ return; } 
        if( obj->state == -2 ){ return; } force_close();
    }
    
    /*─······································································─*/

    socket_t( int df=-1, ulong _size=CHUNK_SIZE ) noexcept {
        obj->fd = df; set_nonbloking_mode();
        set_buffer_size( _size );
    }

    /*─······································································─*/

    virtual void force_close() const noexcept {
        if( obj->state == -3 && obj.count() > 1 ){ resume(); return; }
        if( obj->state == -2 ){ return; } file_t::force_close();
        if( skt->srv == 1 )::shutdown( obj->fd, SHUT_RDWR );
    }

    /*─······································································─*/

    virtual int socket( string_t host, int port ) noexcept { 
        skt->addrlen = sizeof( skt->server_addr );

        if( (obj->fd=::socket( AF, SOCK, PROT )) <=0 )
          { return -1; } set_nonbloking_mode();

        set_buffer_size( CHUNK_SIZE );
        set_reuse_address(1); 
    #ifdef linux
        set_reuse_port(1);
    #endif

        SOCKADDR_IN server, client;
        server.sin_port    = htons ( port );
        server.sin_family  = AF;

             if( host == "0.0.0.0" || host == "globalhost" )       { server.sin_addr.s_addr = INADDR_ANY; }
        else if( host == "255.255.255.255" || host == "broadcast" ){ server.sin_addr.s_addr = INADDR_BROADCAST; } 
        else if( host == "127.0.0.1" || host == "localhost" )      { inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); }
        else                                                       { inet_pton(AF_INET, host.c_str(), &server.sin_addr); }

        skt->server_addr = *((SOCKADDR*) &server);
        skt->client_addr = *((SOCKADDR*) &client);
        skt->len = sizeof( server ); return 1;
    }
    
    /*─······································································─*/

    int _connect() const noexcept { int c=0; if( skt->srv == 1 ){ return -1; }
        return is_blocked( c=::connect( obj->fd, &skt->server_addr, skt->addrlen ) ) ? -2 : c;
    }

    int _accept() const noexcept { int c=0; if( skt->srv == 0 ){ return -1; }
        return is_blocked( c=::accept( obj->fd, &skt->server_addr, &skt->addrlen ) ) ? -2 : c;
    }

    int _bind() const noexcept { int c=0; skt->srv = 1;
        return is_blocked( c=::bind( obj->fd, &skt->server_addr, skt->addrlen ) ) ? -2 : c;
    }

    int _listen( int i=0 ) const noexcept { 
        int c=0; if( skt->srv == 0 ){ return -1; }
        return is_blocked( c=::listen( obj->fd, i ) ) ? -2 : c;
    }

    /*─······································································─*/

    int listen( int i=MAX_SOCKET ) const noexcept { int c=0;
        while( (c=_listen(i))==-2 ){ process::next(); } return c;
    }

    int connect() const noexcept { int c=0;
        while( (c=_connect())==-2 ){ process::next(); } return c;
    }

    int accept() const noexcept { int c=0;
        while( (c=_accept())==-2 ){ process::next(); } return c;
    }

    int bind() const noexcept { int c=0; 
        while( (c=_bind())==-2 ){ process::next(); } return c;
    }
    
    /*─······································································─*/

    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } int c = 0; if( SOCK == SOCK_STREAM ){
            return is_blocked(c=::recv( obj->fd, bf, sx, MSG_DONTWAIT )) ? -2 : c;
        } else { SOCKADDR* cli; if( skt->srv==1 ) cli = &skt->client_addr; else cli = &skt->server_addr;
            return is_blocked(c=::recvfrom( obj->fd, bf, sx, MSG_DONTWAIT, cli, &skt->len )) ? -2 : c;
        }   return -1;
    }
    
    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } int c = 0; if( SOCK == SOCK_STREAM ){
            return is_blocked(c=::send( obj->fd, bf, sx, MSG_DONTWAIT )) ? -2 : c;
        } else { SOCKADDR* cli; if( skt->srv==1 ) cli = &skt->client_addr; else cli = &skt->server_addr;
            return is_blocked(c=::sendto( obj->fd, bf, sx, MSG_DONTWAIT, cli, skt->len )) ? -2 : c;
        }   return -1;
    } 
    
};

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
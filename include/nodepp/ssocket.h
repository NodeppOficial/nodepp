#ifndef NODEPP_SSCOKET
#define NODEPP_SSCOKET

/*────────────────────────────────────────────────────────────────────────────*/

#include "socket.h"
#include "ssl.h"
#include "fs.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

class ssocket_t : public socket_t { 
public: ssocket_t() noexcept : socket_t() {}

    ptr_t<ssl_t> ssl;
    
    /*─······································································─*/

    ssocket_t( ssl_t ctx, int df, ulong _size=CHUNK_SIZE ) noexcept : socket_t() { 
        obj->fd = df; ssl = new ssl_t( ctx, df ); set_buffer_size( _size ); 
                                                  set_nonbloking_mode(); 
    }
    
    /*─······································································─*/

    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } return ssl->_read( bf, sx );
    }
    
    /*─······································································─*/

    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } return ssl->_write( bf, sx );
    }
    
};

}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
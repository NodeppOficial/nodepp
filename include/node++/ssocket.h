#ifndef NODEPP_SSCOKET
#define NODEPP_SSCOKET

/*────────────────────────────────────────────────────────────────────────────*/

#include "socket.h"
#include "ssl.h"
#include "fs.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

class ssocket_t : public socket_t { public:

    ptr_t<ssl_t> ssl;
    
    /*─······································································─*/

    ssocket_t( ssl_t* ctx, int df, ulong _size=CHUNK_SIZE ) noexcept { 
         obj->fd = df; ssl=new ssl_t( ctx, df ); 
         if( ssl->accept() <= 0 ){ close(); }
         set_nonbloking_mode(); set_buffer_size( _size );
    }

    ssocket_t() noexcept = default;
    
    /*─······································································─*/

    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } return ssl->_read( bf, sx );
    }
    
    /*─······································································─*/

    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } return ssl->_write( bf, sx );
    }
    
};

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
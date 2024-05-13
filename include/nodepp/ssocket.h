/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_SSCOKET
#define NODEPP_SSCOKET

/*────────────────────────────────────────────────────────────────────────────*/

#include "socket.h"
#include "ssl.h"
#include "fs.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

class ssocket_t : public socket_t { 
public: ptr_t<ssl_t> ssl;
    
    /*─······································································─*/

    ssocket_t( ssl_t& ctx, int df, ulong size=CHUNK_SIZE ) noexcept 
    :socket_t( df, size ), ssl( new ssl_t( ctx, df ) ) {}

    ssocket_t() noexcept : socket_t() {}
    
    /*─······································································─*/

    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } return ssl->_read( bf, sx );
    }
    
    /*─······································································─*/

    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } return ssl->_write( bf, sx );
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
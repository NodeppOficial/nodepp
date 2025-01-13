/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#pragma once
#include <poll.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { using POLLFD = struct pollfd; class poll_t {
protected:

    struct NODE {
        array_t<POLLFD> ev;
        ptr_t<int>      ls;
    };  ptr_t<NODE>    obj;

public:

    wait_t<ptr_t<int>> onEvent;
    wait_t<int>        onWrite;
    wait_t<int>        onError;
    wait_t<int>        onRead;

public: poll_t() noexcept : obj( new NODE() ) {}

   ~poll_t() noexcept { 
        if ( obj.count() > 1 ){ return; }
        for( auto x : obj->ev ) onError.emit(x.fd); 
    }

    /*─······································································─*/

    ptr_t<int> get_last_poll() const noexcept { return obj->ls; }

    /*─······································································─*/

    int next () noexcept { return emit(); }

    int emit () noexcept { 
        static ulong s=0; static POLLFD x;
    coStart

        if( obj->ev.empty() )                               { coEnd; }
        if( ::poll( obj->ev.data(), obj->ev.size(), 0 )<=0 ){ coEnd; } s = obj->ev.size(); 

        while( s-->0 ){ x = obj->ev[s]; 
              if( x.revents & POLLERR ){ obj->ev.erase(s); onError.emit(x.fd); obj->ls={{-1, x.fd }}; onEvent.emit(obj->ls); coNext; }
            elif( x.revents & POLLIN  ){ obj->ev.erase(s);  onRead.emit(x.fd); obj->ls={{ 0, x.fd }}; onEvent.emit(obj->ls); coNext; }
            elif( x.revents & POLLOUT ){ obj->ev.erase(s); onWrite.emit(x.fd); obj->ls={{ 1, x.fd }}; onEvent.emit(obj->ls); coNext; }
        }

    coStop
    };

    /*─······································································─*/

    bool push_write( const int& fd ) noexcept { 
         for( auto &x: obj->ev ){ if( x.fd==fd ){ return false; } }
	     obj->ev.unshift({ fd, POLLOUT, 0 }); return true;
    }

    bool push_read( const int& fd ) noexcept { 
         for( auto &x: obj->ev ){ if( x.fd==fd ){ return false; } }
         obj->ev.unshift({ fd, POLLIN, 0 }); return true;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
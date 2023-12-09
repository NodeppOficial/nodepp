#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <bluetooth/hci.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/bluetooth.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class bsocket_t : public socket_t { 
private:

    using SOCKADDR_RC = struct sockaddr_rc;

public:

    virtual int socket( string_t host, int port ) noexcept { addrlen = sizeof(server_addr);

        if(( sid = ::socket( AF, SOCK, PROT ) ) <= 0 ) return -1; 
 
        set_buffer_size( CHUNK_SIZE );
        set_nonbloking_mode(1);
        set_reuse_address(1);
        set_reuse_port(1);
        
        SOCKADDR_RC server = {0}, client = {0};
        addr.rc_channel    = (uint8_t) port;
        addr.rc_family     = AF;

        str2ba( host.c_str(), &addr.rc_bdaddr );

        len = sizeof( client );
        server_addr = *( (SOCKADDR*) &server );
        client_addr = *( (SOCKADDR*) &client );
        
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class bluetooth_t {
private:
    using INFO = inquiry_info;
protected:

    struct _str_ { int id, sk; };  
    ptr_t<_str_> obj;

public:

    virtual ~bluetooth_t() noexcept {
        if( obj->count() > 1 ){ return; } 
            close( obj->sk );
    }

    bluetooth_t() : obj( new _str_() ) {
        obj->id = hci_get_route(nullptr); if( obj->id < 0 ) 
        $Error("Failed to open Bluetooth adapter");
    }

    int turn_on() const noexcept { 
        obj->sk = hci_open_dev(obj->id); 
        return obj->sk < 0 ? -1 : 0;
    }

    int turn_off() const noexcept { 
        return hci_close_dev(obj->sk) < 0 ? -1 : 0;
    }

    array_t<string_t> get_devices() const noexcept { 
        
        array_t<string_t> list; INFO* devices = nullptr;
        int num = hci_inquiry( obj->id, 8, 0, nullptr, &devices, IREQ_CACHE_FLUSH );
        if( num < 0 ){ delete devices; return list; }

        for( int i=0; i<num; ++i ){ ptr_t<char> address (18,0);
            ba2str( &(devices[i].bdaddr), &address );
            list.push( address );
        }   
        
        delete devices; return list;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
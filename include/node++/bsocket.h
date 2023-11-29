#ifndef NODEPP_BSOCKET
#define NODEPP_BSOCKET

/*────────────────────────────────────────────────────────────────────────────*/

#include "socket.h"
#include <bluetooth/rfcomm.h>
#include <bluetooth/bluetooth.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

class bsocket_t : public socket_t { public:

    using SOCKADDR_RC = struct sockaddr_rc;

    virtual int socket( string_t host, int port ) noexcept { addrlen = sizeof(server_addr);

        if(( sid = ::socket( AF, SOCK, PROT ) ) <= 0 ) return -1; 
 
        set_buffer_size( CHUNK_SIZE );
        set_nonbloking_mode(1);
        set_reuse_address(1); 

    #ifdef linux
        set_reuse_port(1);
    #endif
        
        SOCKADDR_RC server = {0}, client = {0};
        addr.rc_channel    = (uint8_t) port;
        addr.rc_family     = AF;

        str2ba( host.c_str(), &addr.rc_bdaddr );

        len = sizeof( client );
        server_addr = *( (SOCKADDR*) &server );
        client_addr = *( (SOCKADDR*) &client );
        
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

class bluetooth_t {
private:
    using INFO = inquiry_info;
protected:
    ptr_t<int> id;
    ptr_t<int> sk
public:

    event_t<except_t> onClose;

    bluetooth_t() {
        id = new int( hci_get_route(nullptr) );
        if( *id < 0 ) _Error("Failed to open Bluetooth adapter");
    }

    void turn_on() { sk = new int( hci_open_dev( *id ) ); if( *sk < 0 ) 
        onClose.emit(except_t("can't turn on Bluetooth adapter"));
    }

    void turn_off() { if( hci_close_dev(*sk); < 0 ) 
        onClose.emit(except_t("can't turn off Bluetooth adapter"));
    }

    array_t<char> get_devices(){ 
        array_t<ptr_t<char>> list; ptr_t<INFO> devices;
        int num = hci_inquiry(*id, 8, 0, nullptr, &&devices, IREQ_CACHE_FLUSH);
        
        if( num < 0 ) { 
            onClose.emit(except_t("Device discovery failed"));
            return {};
        }

        for( int i=0; i<num; ++i ) { ptr_t<char> address (18);
            ba2str( &(devices+i)->bdaddr, &address );
            list.push( address );
        }
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <bluetooth/hci.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/bluetooth.h>

#define AF_BTH AF_BLUETOOTH
#define IPPROTO_BTH BTPROTO_RFCOMM

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class bsocket_t : public socket_t { 
private:

    using SOCKADDR_RC = struct sockaddr_rc;

public: bsocket_t() noexcept : socket_t() {}
    
    /*─······································································─*/

    bsocket_t( int df, ulong _size=CHUNK_SIZE ) noexcept : socket_t() { 
        obj->fd = df; set_buffer_size( _size ); 
                      set_nonbloking_mode(); 
    }
    
    /*─······································································─*/

    virtual int socket( const string_t& host, uint8_t port ) noexcept { 
        if( host.empty() ){ _EError(onError,"host is empty"); return -1; }
        skt->addrlen = sizeof( skt->server_addr ); socket::start_device();

        if((obj->fd=::socket( AF, SOCK, PROT )) <= 0 )
          { _EError(onError,"can't initializate socket fd"); return -1; } 
          
        set_buffer_size( CHUNK_SIZE );
        set_nonbloking_mode();
        set_reuse_address(1);

    #if _KERNEL == NODEPP_KERNEL_POSIX
        set_reuse_port(1);
    #endif
        
        SOCKADDR_RC server, client;
        server.rc_channel = port;
        server.rc_family  = AF;

        str2ba( host.c_str(), &addr.rc_bdaddr );
        skt->server_addr = *((SOCKADDR*) &server);
        skt->client_addr = *((SOCKADDR*) &client);
        skt->len = sizeof( server ); return 1;       
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
        if( obj.count() > 1 ){ return; } 
            close( obj->sk );
    }

    bluetooth_t() : obj( new _str_() ) {
        obj->id = hci_get_route(nullptr); if( obj->id < 0 ) 
        _Error("Failed to open Bluetooth adapter");
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
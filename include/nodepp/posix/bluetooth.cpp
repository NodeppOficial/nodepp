#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define AF_BTH      AF_BLUETOOTH
#define IPPROTO_BTH BTPROTO_RFCOMM

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class bsocket_t : public socket_t { 
private:

    using SOCKADDR_RC = struct sockaddr_rc;

public:
    
    /*─······································································─*/

    template < class... T >
    bsocket_t( T... args ) noexcept : socket_t( args... ) {}
    
    /*─······································································─*/

    virtual int socket( const string_t& host, int port ) noexcept { 
        if( host.empty() )
          { process::error(onError,"host is empty"); return -1; }
          
        skt->addrlen = sizeof( skt->server_addr ); socket::start_device();

        if((obj->fd=::socket( AF, SOCK, PROT )) <= 0 )
          { process::error(onError,"can't initializate socket fd"); return -1; } 
          
        set_buffer_size( CHUNK_SIZE );
        set_nonbloking_mode();
        set_reuse_address(1);

    #if _KERNEL == NODEPP_KERNEL_POSIX
        set_reuse_port(1);
    #endif
        
        SOCKADDR_RC server, client;
        memset(&server, 0, sizeof(SOCKADDR_RC));
        memset(&client, 0, sizeof(SOCKADDR_RC));

        server.rc_family  = AF; if( port>0 )
        server.rc_channel = (uint8_t)port;

        str2ba( host.c_str(), &server.rc_bdaddr );
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

    struct NODE { int id, sk; };  
    ptr_t <NODE> obj;

public:

    virtual ~bluetooth_t() noexcept {
        if( obj.count() > 1 ){ return; } 
            close( obj->sk );
    }

    bluetooth_t() : obj( new NODE() ) {
        obj->id = hci_get_route(nullptr); if( obj->id < 0 ) 
        process::error("Failed to open Bluetooth adapter");
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
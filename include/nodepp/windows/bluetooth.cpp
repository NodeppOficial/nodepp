#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <ws2bth.h>
#include <BluetoothAPIs.h>

//#define AF_BTH AF_BTH
#define IPPROTO_BTH BTHPROTO_RFCOMM;

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace { 
    int str2ba( const string_t& straddr, BTH_ADDR* addr ) {
        auto y = regex::split( straddr, ":" );
        for( auto x : y ) { *addr <<= 8;
           *addr|=strtol(x.c_str(),nullptr,16);
        }   return y.size()<6 ? 0 : 1;
    } 
} }

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class bsocket_t : public socket_t { 
private:

    using SOCKADDR_RC = struct sockaddr_rc;

public:
    
    /*─······································································─*/

    template< class... T >
    bsocket_t( T... args ) noexcept : socket_t( args... ) {}
    
    /*─······································································─*/

    virtual int socket( const string_t& host, int port ) noexcept {
        if( host.empty() )
          { process::error(onError,"host is empty"); return -1; }
        
        obj->addrlen = sizeof( obj->server_addr ); socket::start_device();

        if((obj->fd=::socket( AF, SOCK, PROT )) <= 0 )
          { process::error(onError,"can't initializate socket fd"); return -1; } 
          
        set_buffer_size( CHUNK_SIZE );
        set_nonbloking_mode();
        set_reuse_address(1); 

    #if _KERNEL == NODEPP_KERNEL_POSIX
        set_reuse_port(1);
    #endif
        
        SOCKADDR_BTH server, client;
        memset(&server, 0, sizeof(SOCKADDR_BTH));
        memset(&client, 0, sizeof(SOCKADDR_BTH));

        server.addressFamily = AF; if( port>0 )
        server.port          = (ulong) port;

        str2ba( host.c_str(), &server.btAddr );
        obj->server_addr = *((SOCKADDR*) &server);
        obj->client_addr = *((SOCKADDR*) &client);
        obj->len = sizeof( server ); return 1;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class bluetooth_t {
private:
    using INFO = BLUETOOTH_DEVICE_INFO;

protected:
    struct NODE {
        HBLUETOOTH_RADIO_FIND hFind;
        HANDLE hRadio;
    };  ptr_t<NODE> obj;

public:

    virtual ~bluetooth_t() noexcept {
        if( obj.count() > 1 ){ return; }
            CloseHandle( obj->hRadio );
    }

    bluetooth_t() : obj( new NODE() ) {
        obj->hFind = BluetoothFindFirstRadio( nullptr, &obj->hRadio );
        if( obj->hFind == nullptr )
          { process::error("Failed to open Bluetooth adapter"); }
    }

    int turn_on() const noexcept {
        return BluetoothEnableDiscovery(obj->hRadio, TRUE) != ERROR_SUCCESS ? -1 : 0;
    }

    int turn_off() const noexcept {
        return BluetoothEnableDiscovery( obj->hRadio, FALSE ) != ERROR_SUCCESS ? -1 : 0;
    }

    array_t<string_t> get_devices() { array_t<string_t> list;

        BLUETOOTH_DEVICE_SEARCH_PARAMS searchParams = { sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS) };
        BLUETOOTH_DEVICE_INFO          deviceInfo   = { sizeof(BLUETOOTH_DEVICE_INFO) };
        HBLUETOOTH_DEVICE_FIND         hFind        = BluetoothFindFirstDevice( &searchParams, &deviceInfo );

        if( hFind == nullptr ) { BluetoothFindDeviceClose( hFind ); return list; }

        do { 
            ptr_t<char> name ( wcslen( deviceInfo.szName ) + 1, 0 );
            wcstombs( &name, deviceInfo.szName, name.size() );
            list.push({ &name, name.size() });
        } while ( BluetoothFindNextDevice( hFind, &deviceInfo ) );

        BluetoothFindDeviceClose( hFind ); return list;
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/
#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <ws2bth.h>
#include <BluetoothAPIs.h>

namespace { namespace nodepp {
int str2ba( const string_t& straddr, const BTH_ADDR& addr ) {
    auto y = regex::split( straddr, ":" );
    for( auto x : y ) { addr <<= 8;
        addr|=strtol(x,nullptr,16);
    }   return y.size()<6 ? 0 : 1;
} 
} }

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class bsocket_t : public socket_t { 
public: bsocket_t() noexcept : socket_t() {}

    using SOCKADDR_RC = struct sockaddr_rc;

    virtual int socket( const string_t& host, int port ) noexcept { addrlen = sizeof(server_addr);
        obj->addrlen = sizeof( obj->server_addr );

        if((obj->fd=::socket( AF, SOCK, PROT )) == INVALID_SOCKET )
          { return -1; }
 
        set_buffer_size( CHUNK_SIZE );
        set_reuse_address( 1 ); 
        
        SOCKADDR_BTH server= {0}, client = {0};
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
    using INFO = BLUETOOTH_DEVICE_INFO;

protected:
    struct _str_ {
        HBLUETOOTH_RADIO_FIND hFind;
        HANDLE hRadio;
    };  ptr_t<_str_> obj;

public:

    virtual ~bluetooth_t() noexcept {
        if( obj->count() > 1 ){ return; }
            CloseHandle( obj->hRadio );
    }

    bluetooth_t() : obj( new _str_() ) {
        obj->hFind = BluetoothFindFirstRadio( nullptr, &obj->hRadio );
        if( obj->hFind == nullptr ){ $Error("Failed to open Bluetooth adapter"); }
    }

    int turn_on() const noexcept {
        return BluetoothEnableDiscovery(obj->hRadio, TRUE) != ERROR_SUCCESS ? -1 : 0;
    }

    int turn_off() const noexcept {
        BluetoothEnableDiscovery( obj->hRadio, FALSE ) != ERROR_SUCCESS ? -1 : 0
    }

    array_t<string_t> get_devices() { array_t<string_t> list; DWORD num = 0;
        BLUETOOTH_DEVICE_INFO deviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO) };
        HBLUETOOTH_DEVICE_FIND hFind = BluetoothFindFirstDevice(&deviceInfo, &hFind);

        if( hFind == nullptr ) { BluetoothFindDeviceClose( hFind ); return list; }

        do { list.push( deviceInfo.szName, (ulong) BLUETOOTH_MAX_NAME_SIZE );
        } while ( BluetoothFindNextDevice( hFind, &deviceInfo ) );

        BluetoothFindDeviceClose( hFind ); return list;
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/
#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <iomanip>
#include <Windows.h>
#include <wlanapi.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "iphlpapi.lib")

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class wifi_t {
private:

    using INFO = WLAN_AVAILABLE_NETWORK;

protected:

    struct _str_ {
        HANDLE hClient;
    };  ptr_t<_str_> obj;

public:

    virtual ~wifi_t() noexcept {
        if( obj.count() > 1 ){ return; } 
        WlanCloseHandle( obj->hClient, NULL );
    }

    wifi_t() : obj( new _str_() ) {
        DWORD dwMaxClient = 2; DWORD dwCurVersion;
        DWORD dwResult = WlanOpenHandle(dwMaxClient, nullptr, &dwCurVersion, &obj->hClient);
        if( dwResult != ERROR_SUCCESS ){ _Error("Failed to open Wi-Fi adapter"); }
    }

    int turn_on() const noexcept {
        DWORD dwResult = WlanScan(obj->hClient, nullptr, nullptr, nullptr, nullptr);
        if( dwResult != ERROR_SUCCESS ){ return -1; } return 0;
    }

    int turn_off() const noexcept {
        DWORD dwResult = WlanDisconnect(obj->hClient, nullptr, nullptr);
        if( dwResult != ERROR_SUCCESS ){ return -1; } return 0;
    }

    string_t get_hostname() const noexcept {
        string_t result; char hostName[256];
        socket::start_device();
        
        if( gethostname(hostName,sizeof(hostName)) == 0 ){
            struct hostent* hostEntry; hostEntry = gethostbyname(hostName);
            if( hostEntry != nullptr ){
                struct in_addr** addrList = reinterpret_cast<struct in_addr**>(hostEntry->h_addr_list);
                if( addrList[0] != nullptr ){ result = inet_ntoa(*addrList[0]); }
            }
        }

        return result;
    }

    array_t<string_t> get_device_list() const noexcept {
        ULONG bufferSize = 0; array_t<string_t> device;
        DWORD result     = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, nullptr, &bufferSize);
        
        if (result == ERROR_BUFFER_OVERFLOW) {
            IP_ADAPTER_ADDRESSES* adapterAddresses = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(new char[bufferSize]);
            result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, adapterAddresses, &bufferSize);
            if ( result == ERROR_SUCCESS ) {
                IP_ADAPTER_ADDRESSES* adapter = adapterAddresses; while ( adapter!=nullptr ) {
                    device.push( adapter->AdapterName );
                    adapter = adapter->Next;
                }
            }   delete[] reinterpret_cast<char*>(adapterAddresses);
        }

        return device;
    }

    array_t<string_t> get_ssid_list() const noexcept {
        array_t<string_t> list; PWLAN_AVAILABLE_NETWORK_LIST pAvailableNetworkList = nullptr;

        DWORD dwResult = WlanGetAvailableNetworkList(obj->hClient, nullptr, 0, nullptr, &pAvailableNetworkList);
        if( dwResult != ERROR_SUCCESS ){ WlanFreeMemory(pAvailableNetworkList); return list; }

        for( DWORD i = 0; i < pAvailableNetworkList->dwNumberOfItems; i++ ){
            PWLAN_AVAILABLE_NETWORK pNetwork = &pAvailableNetworkList->Network[i];
            string_t ssid = (char*) pNetwork->dot11Ssid.ucSSID; list.push( ssid );
        }

        WlanFreeMemory(pAvailableNetworkList); return list;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
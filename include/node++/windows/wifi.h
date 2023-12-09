#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <iomanip>
#include <Windows.h>
#include <wlanapi.h>
#pragma comment(lib, "wlanapi.lib")

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
        if( dwResult != ERROR_SUCCESS ){ $Error("Failed to open Wi-Fi adapter"); }
    }

    int turn_on() const noexcept {
        DWORD dwResult = WlanScan(obj->hClient, nullptr, nullptr, nullptr, nullptr);
        if( dwResult != ERROR_SUCCESS ){ return -1; } return 0;
    }

    int turn_off() const noexcept {
        DWORD dwResult = WlanDisconnect(obj->hClient, nullptr, nullptr);
        if( dwResult != ERROR_SUCCESS ){ return -1; } return 0;
    }

    array_t<string_t> get_networks() const noexcept {
        array_t<string_t> list; PWLAN_AVAILABLE_NETWORK_LIST pAvailableNetworkList = nullptr;

        DWORD dwResult = WlanGetAvailableNetworkList(obj->hClient, nullptr, 0, nullptr, &pAvailableNetworkList);
        if( dwResult != ERROR_SUCCESS ){ WlanFreeMemory(pAvailableNetworkList); return list; }

        for( DWORD i = 0; i < pAvailableNetworkList->dwNumberOfItems; i++ ){
            PWLAN_AVAILABLE_NETWORK pNetwork = &pAvailableNetworkList->Network[i];
            string_t ssid = (char*)pNetwork->dot11Ssid.ucSSID; list.push( ssid );
        }

        WlanFreeMemory(pAvailableNetworkList); return list;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
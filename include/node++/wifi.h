#ifndef NODEPP_WIFI
#define NODEPP_WIFI

/*────────────────────────────────────────────────────────────────────────────*/

#include <iomanip>
#include <iwlib.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {
    class wifi_t {
    private:
        using INFO = wireless_scan_head;

    protected:
        struct _str_ { int sockfd; };
        ptr_t<_str_> obj;

    public:

        virtual ~wifi_t() noexcept {
            if( obj.count() > 1 ){ return; } 
            iw_sockets_close( obj->sockfd ); 
        }

        wifi_t() noexcept : obj( new _str_() ) {
            obj->sockfd = iw_sockets_open(); if( obj->sockfd<0 )
                _Error("Failed to open Wi-Fi adapter");
        }

        int turn_on() const noexcept {
            // Perform the necessary operations to turn on Wi-Fi on Linux
            return 0;
        }

        int turn_off() const noexcept {
            // Perform the necessary operations to turn off Wi-Fi on Linux
            return 0;
        }

        array_t<string_t> get_networks() noexcept {
            array_t<string_t> list;
            wireless_scan_head scan_head;
            wireless_scan *scan_result = nullptr;

            if( iw_scan(obj->sockfd,"wlan0",0,&scan_result)<0 ) { return list; }

            iw_bss *bss = nullptr;

            for (bss = scan_result->result; bss; bss = bss->next) {
                char ssid[IW_ESSID_MAX_SIZE + 1] = {0};
                iw_ether_ntop(&bss->bssid, ssid);
                list.push( ssid );
            }
            
            return list;
        }
    };
}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
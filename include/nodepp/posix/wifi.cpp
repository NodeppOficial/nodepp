#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <iomanip>
#include <iwlib.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class wifi_t {
private:

    using INFO = wireless_scan_head;

protected:

    struct NODE { int sockfd; };
    ptr_t <NODE> obj;

public:

    virtual ~wifi_t() noexcept {
        if( obj.count() > 1 ){ return; } 
        iw_sockets_close( obj->sockfd ); 
    }

    wifi_t() : obj( new NODE() ) { 
        obj->sockfd = iw_sockets_open(); if( obj->sockfd<0 )
             process::error("Failed to open Wi-Fi adapter");
    }

    int turn_on( const string_t& device ) const noexcept {
        return system(string::format("ifconfig %s down",device.data()).c_str());
    }

    int turn_off( const string_t& device ) const noexcept {
        return system(string::format("ifconfig %s up",device.data()).c_str());
    }

    string_t get_hostname( const string_t& device ) const noexcept {
        struct ifaddrs *interfaces = NULL;
        struct ifaddrs *temp_addr  = NULL;
        string_t result;

        if ( getifaddrs(&interfaces) == 0 ) {
            temp_addr = interfaces; while (temp_addr!=NULL) {
                if ( temp_addr->ifa_addr->sa_family == AF_INET ) {
                if ( strcmp(temp_addr->ifa_name,device.c_str()) == 0 ) {
                    result = inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
                }}  temp_addr = temp_addr->ifa_next;
            }
        }

        freeifaddrs(interfaces); return result;
    }

    array_t<string_t> get_device_list() const noexcept {
        struct ifaddrs *interfaces = NULL;
        struct ifaddrs *temp_addr  = NULL;
        array_t<string_t> result;

        if ( getifaddrs(&interfaces) == 0 ) {
            temp_addr = interfaces; while (temp_addr!=NULL) {
                if ( temp_addr->ifa_addr->sa_family == AF_INET ) {
                    result.push( temp_addr->ifa_name );
                }   temp_addr = temp_addr->ifa_next;
            }
        }

        freeifaddrs(interfaces); return result;
    }

    array_t<string_t> get_ssid_list( const string_t& device ) const noexcept {
        wireless_scan     *scan_result = nullptr;
        iw_bss            *bss         = nullptr;
        wireless_scan_head scan_head;
        array_t<string_t>  list;

        if( iw_scan(obj->sockfd,device.c_str(),0,&scan_result)<0 ) { return list; }

        for( bss = scan_result->result; bss; bss = bss->next ) {
            char ssid[IW_ESSID_MAX_SIZE + 1];
            iw_ether_ntop(&bss->bssid, ssid);
            list.push( ssid );
        }
        
        return list;
    }
};}

/*────────────────────────────────────────────────────────────────────────────*/
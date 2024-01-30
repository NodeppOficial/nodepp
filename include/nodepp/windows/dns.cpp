#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <iomanip>
#include <Windows.h>
#include <wlanapi.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wlanapi.lib")

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace dns { 
    
    string_t ipv4 = "([0-9]{1,3}[.]){3}[0-9]{1,3}";
    string_t ipv6 = "([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}"; 
    
    /*─······································································─*/

    string_t lookup( string_t host ) { socket::start_device();

          if( host == "255.255.255.255" || host == "broadcast" ) { return "255.255.255.255"; } 
        elif( host == "127.0.0.1" || host == "localhost" )       { return "127.0.0.1";       } 
        elif( host == "0.0.0.0" || host == "globalhost" )        { return "0.0.0.0";         }

        if( url::is_valid(host) ){ host = url::hostname(host); }
        struct hostent* host_info = gethostbyname(host.c_str());

        if( host_info != NULL ){
            struct in_addr** address_list = (struct in_addr**)host_info->h_addr_list;
            int address_size = host_info->h_length;
            for( int x = 0; x < address_size; x++ ) {
             if( address_list[x] == nullptr ){ continue; }
                 return inet_ntoa(*address_list[x]);
            }
        }

        return "";
    }
    
    /*─······································································─*/

    string_t get_hostname(){
        string_t result; char hostName[256]; socket::start_device();

        if( gethostname(hostName,sizeof(hostName)) == 0 ){
            struct hostent* hostEntry; hostEntry = gethostbyname(hostName);
            if( hostEntry != nullptr ){
                struct in_addr** addrList = reinterpret_cast<struct in_addr**>(hostEntry->h_addr_list);
                if( addrList[0] != nullptr ){ result = inet_ntoa(*addrList[0]); }
            }
        }

        return result;
    }
    
    /*─······································································─*/

    bool is_ipv4( string_t URL ){ return regex::test( URL, ipv4 ) ? 1 : 0; }

    bool is_ipv6( string_t URL ){ return regex::test( URL, ipv6 ) ? 1 : 0; }
    
    /*─······································································─*/

    bool is_ip( string_t URL ){ 
        if( URL.empty() )     { return 0; }
        if( is_ipv4(URL) > 0 ){ return 1; }
        if( is_ipv6(URL) > 0 ){ return 1; } return 0;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/
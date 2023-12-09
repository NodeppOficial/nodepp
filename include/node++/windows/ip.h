#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace ip { 
    
    string_t ipv4 = "([0-9]{1,3}[.]){3}[0-9]{1,3}";
    string_t ipv6 = "([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}"; 
    
    /*─······································································─*/

    string_t lookup( string_t host ) {

             if( host == "255.255.255.255" || host == "broadcast" ) { return "255.255.255.255"; }
        else if( host == "127.0.0.1" || host == "localhost" )       { return "127.0.0.1";       }
        else if( host == "0.0.0.0" || host == "globalhost" )        { return "0.0.0.0";         }

        if( url::is_valid(host) ){ host = url::hostname(host); }

        struct addrinfo* result = nullptr; struct addrinfo hints;
               ZeroMemory(&hints, sizeof(hints));

        hints.ai_family   = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        int status = getaddrinfo(host.c_str(), nullptr, &hints, &result);
        if( status != 0 ) { return ""; } string_t ipAddress;
        
        for( struct addrinfo* ptr=result; ptr!=nullptr; ptr=ptr->ai_next ) { void* addr;

            if( ptr->ai_family == AF_INET ) {
                struct sockaddr_in* ipv4 = (struct sockaddr_in*)ptr->ai_addr;
                addr = &(ipv4->sin_addr);
            } else {
                struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)ptr->ai_addr;
                addr = &(ipv6->sin6_addr);
            }

            char ipBuffer[INET6_ADDRSTRLEN];
            inet_ntop(ptr->ai_family, addr, ipBuffer, sizeof(ipBuffer));
            ipAddress = (string_t){ ipBuffer, (ulong) INET6_ADDRSTRLEN }; break;
        }

        freeaddrinfo(result); return ipAddress;
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
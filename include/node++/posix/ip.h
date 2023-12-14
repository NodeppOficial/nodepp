#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <arpa/inet.h>
#include <netdb.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace {
    using HOSTENT = struct hostent;
    using INADDR  = struct in_addr;
}

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

    bool is_ipv4( const string_t& URL ){ return regex::test( URL, ipv4 ) ? 1 : 0; }

    bool is_ipv6( const string_t& URL ){ return regex::test( URL, ipv6 ) ? 1 : 0; }
    
    /*─······································································─*/

    bool is_ip( const string_t& URL ){ 
        if( URL.empty() )     { return 0; }
        if( is_ipv4(URL) > 0 ){ return 1; }
        if( is_ipv6(URL) > 0 ){ return 1; } return 0;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/
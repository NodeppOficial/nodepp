#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <emscripten.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace ip { 
    
    string_t ipv4 = "([0-9]{1,3}[.]){3}[0-9]{1,3}";
    string_t ipv6 = "([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}"; 
    
    /*─······································································─*/

    string_t lookup( string_t host ){
        
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
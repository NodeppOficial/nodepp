#ifndef NODEPP_HTTPS
#define NODEPP_HTTPS

/*────────────────────────────────────────────────────────────────────────────*/

#include "http.h"
#include "tls.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

using https_header_t = map_t< string_t, string_t >;

/*────────────────────────────────────────────────────────────────────────────*/

class https_t : public ssocket_t {
protected:

    bool           has_header=0;
    string_t       version;
    
public:

    https_header_t headers;
    int            status;
    query_t        query;

    string_t       protocol;
    string_t       search;
    string_t       method;
    string_t       path;
    string_t       url;
    
    /*─······································································─*/

    https_t() noexcept : ssocket_t() {}

    https_t( const ssocket_t& oth ) noexcept : ssocket_t(oth) {}
    
    /*─······································································─*/

    string_t get_version() const noexcept { return version; }
    
    /*─······································································─*/

    int read_header() noexcept {
        
        if( !is_available() ){ return -1; }
        static array_t<string_t> init; 
        string_t base, line, a, b;
        int idx;

    _Start

        base = read_line(); protocol = "HTTPS";
          if( !regex::test( base,"HTTP/\\d\\.\\d" ) ) _End; 
        init = regex::match_all( base, "[^\\s\t\r\n ]+" ); _Next;
        
        if( !regex::test( init[1], "^\\d+" ) ) {
            auto idx = init[1].index_of([]( char x ){ return x=='?'; });
              
            if( idx > 0 ){
                path   = init[1].slice( 0,idx-1 );
                search = init[1].slice( idx + 1 );
                query  = search_params::parse(search);
            } else {
                path   = init[1];
            }

            version = init[2]; method = init[0];
            url     = string::format( "https://%s%s%s", (char*)headers["Host"], (char*)path, (char*)search );
        } else {
            version = init[0]; status = string::to_int(init[1]);
        }   _Next;

        do{ line = read_line(); idx = line.index_of([]( char x ){ return x==':'; });
            if( idx < 0 ) break; a = line.slice( 0,idx-1 ).to_capital_case();
                                 b = line.slice( idx + 1 ); headers[a] = b;
        } while ( true ); _Return(0); _Goto(0);

    _Stop
    }
    
    /*─······································································─*/

    void write_headers( uint status, https_header_t headers ) noexcept {
        if( has_header == 1 ){ return; } has_header = 1;
        string_t res; res += string::format("%s %u %s\r\n",(char*)version,status,(char*)HTTP_NODEPP::_get_http_status(status));
        for( auto x:headers ){ res += string::format("%s: %s\r\n",(char*)x.first.to_capital_case(),(char*)x.second); }
                               res += "\r\n"; write( res ); if( method == "HEAD" ){ close(); }
    }
    
    /*─······································································─*/

    void write_headers( string_t method, string_t path, string_t version, https_header_t headers ) noexcept {
        if( has_header == 1 ){ return; } has_header = 1; 
        string_t res; res += string::format("%s %s %s\r\n",(char*)method,(char*)path,(char*)version);
        for( auto x:headers ){ res += string::format("%s: %s\r\n",(char*)x.first.to_capital_case(),(char*)x.second); }
                               res += "\r\n"; write( res );
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

namespace https {

    template< class T > tls_t server( const T& cb, ssl_t* ctx, agent_t* opt=nullptr ){
        return tls_t([=]( https_t cli ){ int c=0;
            while(( c=cli.read_header() ) > 0 )
                 { process::next(); }
            if( c==0 ){ cb( cli ); }
            else{ cli.close(); }
        }, ctx, opt ); 
    }
    
    /*─······································································─*/

    template< class T > tls_t client( const T& cb, ssl_t* ctx, agent_t* opt=nullptr ){ 
        auto client = tls::client( ctx, opt );
        client.onOpen( cb ); return client;
    }
    
}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
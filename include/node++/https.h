#ifndef NODEPP_HTTPS
#define NODEPP_HTTPS

/*────────────────────────────────────────────────────────────────────────────*/

#include "http.h"
#include "tls.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { using https_header_t = map_t< string_t, string_t >;

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

    $Start

        base = read_line(); protocol = "HTTPS";
          if( !regex::test( base,"HTTP/\\d\\.\\d" ) ) $End; 
        init = regex::match_all( base, "[^\\s\t\r\n ]+" ); $Next;
        
        if( !regex::test( init[1], "^\\d+" ) ) {
            auto idx = init[1].index_of([]( char x ){ return x=='?'; });
              
            if( idx > 0 ){
                path   = init[1].slice( 0,idx );
                search = init[1].slice(   idx );
                query  = search_params::parse(search);
            } else {
                path   = init[1];
            }

            version = init[2]; method = init[0];
            url     = string::format( "https://%s%s%s", (char*)headers["Host"], (char*)path, (char*)search );
        } else {
            version = init[0]; status = string::to_int(init[1]);
        }   $Next;

        do{ line = read_line();   idx = line.index_of([]( char x ){ return x==':'; });
            if( idx < 0 ){ break; } a = line.slice( 0,idx ).to_capital_case();
                                    b = line.slice( idx+2 ); headers[a] = b;
        } while ( true ); $Return(0); $Goto(0);

    $Stop
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

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { struct https_fetch_t {

    ssl_t*     ssl = nullptr;
    file_t*   body = nullptr;
    agent_t* agent = nullptr;
    
    /*─······································································─*/

    http_header_t headers;
    
    /*─······································································─*/

    string_t     url;
    string_t  method = "GET";
    string_t version = "HTTP/1.0";
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace https {

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
    
    /*─······································································─*/
    
    promise_t<https_t,except_t> fetch ( https_fetch_t cfg ) { ptr_t<https_fetch_t> _cfg = new https_fetch_t( cfg );
    return promise_t<https_t,except_t>([=]( function_t<void,https_t> res, function_t<void,except_t> rej ){

        if( !url::is_valid( _cfg->url ) ){ rej(except_t("invalid URL")); return; }
        
        url_t    uri = url::parse( _cfg->url );
        string_t dip = ip::lookup( uri.hostname );
        string_t dir = uri.pathname + uri.search + uri.hash;

        if( uri.protocol != "https" ){ rej(except_t("wrong protocol")); return; }
        if( !ip::is_ip( dip ) ){ rej(except_t("couldn't get IP")); return; }

        _cfg->headers["Host"] = uri.hostname;
       
        auto client = https::client([=]( https_t cli ){ int c = 0;
            cli.write_headers( _cfg->method, dir, _cfg->version, _cfg->headers );
            while( _cfg->body!=nullptr && _cfg->body->is_available() )
                 { cli.write( _cfg->body->read() ); }
            while((c=cli.read_header())>0 ){ process::next(); }
            if( c==0 ){ res( cli ); return; } else { 
                rej(except_t("couldn't connect to the server")); return; 
            }
        }, _cfg->ssl, _cfg->agent );

        client.onError([=]( except_t error ){ rej(error); });
        client.connect( dip, uri.port );

    }); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
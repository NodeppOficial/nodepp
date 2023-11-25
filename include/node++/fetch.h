#ifndef NODEPP_FETCH
#define NODEPP_FETCH

/*────────────────────────────────────────────────────────────────────────────*/

#include "ip.h"
#include "http.h"
#include "https.h"
#include "promise.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

struct fetch_t {

    ssl_t*     ssl = nullptr;
    file_t*   body = nullptr;
    agent_t* agent = nullptr;
    
    /*─······································································─*/

    http_header_t headers;
    
    /*─······································································─*/

    string_t     url;
    string_t  method = "GET";
    string_t version = "HTTP/1.0";
    
};

/*────────────────────────────────────────────────────────────────────────────*/

namespace fetch { 

    promise_t<http_t,except_t> http ( fetch_t cfg ) { ptr_t<fetch_t> _cfg = new fetch_t( cfg );
    return promise_t<http_t,except_t>([=]( function_t<void,http_t> res, function_t<void,except_t> rej ){

        if( !url::is_valid( _cfg->url ) ){ rej(except_t("invalid URL")); return; }
        
        url_t    uri = url::parse( _cfg->url );
        string_t dip = ip::lookup( uri.hostname );
        string_t dir = uri.pathname + uri.search + uri.hash;

        if( uri.protocol != "http" ){ rej(except_t("wrong protocol")); return; }
        if( !ip::is_ip( dip ) ){ rej(except_t("couldn't get IP")); return; }

        _cfg->headers["Host"] = uri.hostname;
       
        auto client = http::client([=]( http_t cli ){ int c = 0;
            cli.write_headers( _cfg->method, dir, _cfg->version, _cfg->headers );
            while( _cfg->body!=nullptr && _cfg->body->is_available() )
                 { cli.write( _cfg->body->read() ); }
            while(( c=cli.read_header() )>0 ){ process::next(); }
            if( c==0 ){ res( cli ); return; } else { 
                rej(except_t("couldn't connect to the server")); return; 
            }
        }, _cfg->agent );

        client.onError([=]( except_t error ){ rej(error); });
        client.connect( dip, uri.port );

    }); }
    
    /*─······································································─*/

    promise_t<https_t,except_t> https ( fetch_t cfg ) { ptr_t<fetch_t> _cfg = new fetch_t( cfg );
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

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
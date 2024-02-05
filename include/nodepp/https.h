#ifndef NODEPP_HTTPS
#define NODEPP_HTTPS

/*────────────────────────────────────────────────────────────────────────────*/

#include "http.h"
#include "tls.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class https_t : public ssocket_t, public NODEPP_GENERATOR_BASE {
protected:

    bool      has_header=0;
    string_t  version;
    
public:

    header_t  headers;
    int       status;
    query_t   query;

    string_t  protocol;
    string_t  search;
    string_t  method;
    string_t  path;
    string_t  url;
    
    /*─······································································─*/

    template< class... T > 
    https_t( const T&... args ) noexcept : ssocket_t( args... ) {}
    
    /*─······································································─*/

    void     set_version( const string_t& msg ) noexcept { version = msg; }

    string_t get_version() const noexcept { return version; }

    /*─······································································─*/

    int read_header() noexcept {
        static array_t<string_t> init; 
        string_t base, line, a, b;
        int idx;
    _GStart

        if( !is_available() )                              _End;
        base = read_line(); protocol = "HTTPS";
        if( !regex::test( base,"HTTP/\\d\\.\\d" ) )        _End; 
        init = regex::match_all( base, "[^\\s\t\r\n ]+" ); _Next;
        
        if( !regex::test( init[1], "^\\d+" ) ) {
            auto idx = init[1].index_of([]( char x ){ return x=='?'; });
              
            if( idx > 0 ){
                path   = init[1].slice( 0,idx );
                search = init[1].slice(   idx );
                query  = search_params::parse(search);
            } else {
                path   = init[1];
            }

            method  = init[0]; if( version.empty() ) version = init[2];
            url     = string::format( "https://%s%s%s", (char*)headers["Host"], (char*)path, (char*)search );
        } else {
            version = init[0]; status = string::to_int(init[1]);
        }   _Next;

        do {  line = read_line(); idx = line.index_of([]( char x ){ return x==':'; });
            if( idx < 0 ){ break; } a = line.slice( 0,idx ).to_capital_case();
                                    b = line.slice( idx+2 ); 
                                    headers[a] = b;
        } while ( true ); _Set(0); return 0;

    _GStop
    }
    
    /*─······································································─*/

    void write_headers( uint status, const header_t& headers ) noexcept {
        if( has_header == 1 ){ return; } has_header = 1;
        string_t res; res += string::format("%s %u %s\r\n",(char*)version,status,(char*)HTTP_NODEPP::_get_http_status(status));
        for( auto x:headers ){ res += string::format("%s: %s\r\n",(char*)x.first.to_capital_case(),(char*)x.second); }
                               res += "\r\n"; write( res ); if( method == "HEAD" ){ close(); }
    }
    
    /*─······································································─*/

    void write_headers( const string_t& method, const string_t& path, const string_t& version, const header_t& headers ) noexcept {
        if( has_header == 1 ){ return; } has_header = 1; 
        string_t res; res += string::format("%s %s %s\r\n",(char*)method,(char*)path,(char*)version);
        for( auto x:headers ){ res += string::format("%s: %s\r\n",(char*)x.first.to_capital_case(),(char*)x.second); }
                               res += "\r\n"; write( res );
    }
    
    /*─······································································─*/

    void write_filestream( const string_t& body, const file_t& file ){
        if ( !body.empty() ){ write( body ); return; } 
        while( file.is_available() ){ 
            string_t s = file.read();
            if( s.empty() ){ break; } write( s ); 
        }
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace https {

    template< class T > tls_t server( T cb, ssl_t* ctx, agent_t* opt=nullptr ){
        return tls_t([=]( https_t cli ){ int c=0;
            while(( c=cli.read_header() ) == 1 )
                 { process::next(); }
            if( c==0 ){ cb( cli ); }
            else { cli.close(); }
        }, ctx, opt ); 
    }
    
    /*─······································································─*/
    
    promise_t<https_t,except_t> fetch ( const fetch_t& cfg, ssl_t* ctx, agent_t* opt=nullptr ) { 
           ptr_t<fetch_t> _cfg = new fetch_t( cfg );
        if( ctx == nullptr ) _Error( "Invalid SSL Contenx" );
           ptr_t<agent_t> agn = new agent_t( opt==nullptr?agent_t():*opt ); 
           ptr_t<ssl_t>   ssl = new ssl_t  ( *ctx );
    return promise_t<https_t,except_t>([=]( function_t<void,https_t> res, function_t<void,except_t> rej ){

        if( !url::is_valid( _cfg->url ) ){ rej(except_t("invalid URL")); return; }
        
        url_t    uri = url::parse( _cfg->url );
        string_t dip = uri.hostname ;
        string_t dir = uri.pathname + uri.search + uri.hash;
       
        auto client = tls_t ([=]( https_t cli ){ int c = 0;
            cli.write_headers( _cfg->method, dir, _cfg->version, _cfg->headers );
            cli.write_filestream( _cfg->body, _cfg->file );
            while(( c=cli.read_header() )>0 ){ process::next(); }
            if( c==0 ){ res( cli ); return; } else { 
                rej(except_t("couldn't connect to the server")); 
                cli.close(); return; 
            }
        }, &ssl, &agn );

        client.onError([=]( except_t error ){ rej(error); });
        client.connect( dip, uri.port );

    }); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
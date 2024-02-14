#ifndef NODEPP_URL
#define NODEPP_URL

/*────────────────────────────────────────────────────────────────────────────*/

#include "query.h"
#include "regex.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

ptr_t<string_t> prot ({ "https", "http", "wss", "ws", "ftp", "ftps", "tcp", "tls", "udp", "dtls" });
ptr_t<int>      prts ({  443   ,  80   ,  443 ,  80 ,  21  ,  21   ,  80  ,  443 ,  80  ,  443   });

/*────────────────────────────────────────────────────────────────────────────*/

struct url_t {
    string_t hostname;
    string_t protocol;
    string_t pathname;
    string_t search;
    string_t origin;
    query_t  query;
    string_t auth;
    string_t host;
    string_t hash;
    string_t href;
    string_t user;
    string_t pass;
    string_t path;
    int port;
};

/*────────────────────────────────────────────────────────────────────────────*/

namespace url {

    bool is_valid( const string_t& URL ){
        return regex::test( URL, "^\\w+://([^.]+)", "i" );
    }
    
    /*─······································································─*/

    string_t protocol( const string_t& URL ){ 
        string_t null; regex_t _a("^[^:]+");
        if( !is_valid(URL) || !_a.test( URL ) ) 
          { return null; } null = _a.match( URL );
            return null;
    }
    
    /*─······································································─*/

    string_t auth( const string_t& URL ){ string_t null; 
        regex_t _a("//[^@/]+@"), _b("@"), _c("[@/]+"), _d(":");
        if( !is_valid(URL) || !_a.test( URL ) ) 
          { return null; } null = _a.match( URL );
        if( !_b.test( null ) ){ return ""; }
        if( !_d.test( null ) ){ return ""; }
        if( null.size() <= 4 ){ return ""; }
            return _c.replace_all( null, "" );
    }

    string_t user( const string_t& URL ){ string_t null; 
        auto data = string::split( auth( URL ), ':' );
        if( data.size() != 2 ){ return null; } return data[0];
    }

    string_t pass( const string_t& URL ){ string_t null; 
        auto data = string::split( auth( URL ), ':' );
        if( data.size() != 2 ){ return null; } return data[1];
    }
    
    /*─······································································─*/

    string_t hash( const string_t& URL ){ 
        string_t null; regex_t _a("#[^?]*");
        if( !is_valid(URL) || !_a.test( URL ) ) 
          { return null; } return _a.match( URL );
    }

    string_t search( const string_t& URL ){ 
        string_t null; regex_t _a("\\?[^#]*");
        if( !is_valid(URL) || !_a.test( URL ) ) 
          { return null; } return _a.match( URL );
    }
    string_t origin( const string_t& URL ){
        string_t null; regex_t _a("^[^/]+//[^/?#]+");
        if( !is_valid(URL) || !_a.test( URL ) )
          { return null; } return _a.match( URL );
    }

    string_t path( const string_t& URL ){
        string_t null; regex_t _a("/+[^?/#]+");
        if( !is_valid(URL) || !_a.test(URL) ){ return ""; }
	    auto vec = _a.match_all( URL );
	if( vec.size() <= 1 ) return "/";
	    vec.shift(); return vec.join("");
    }

    string_t host( const string_t& URL ){ string_t null; 
        regex_t _a("/\\w[^/#?]+"), _c("/"), _d("[^@]+@|@");
        if( !is_valid(URL) ){ return null; }
            null = _a.match( URL );
        if( _d.test( null ) )
          { null = _d.replace_all( null, "" ); }
            return _c.replace_all( null, "" );
    }

    string_t hostname( const string_t& URL ){ 
        string_t null = host(URL); regex_t _a("[^:]+");
        if( !is_valid(URL) || !_a.test( null ) ) 
          { return null; } return _a.match( null );
    }
    
    /*─······································································─*/

    int port( const string_t& URL ){ regex_t _a(":\\d+$"), _b("[^\\d]+");
        string_t _host = host( URL ); string_t _prot = protocol( URL );
        if( _a.test( _host ) ){
            string_t _port = _a.match( _host );
                   _port = _b.replace_all( _port, "" );
                   return string::to_uint( _port );
        } else {
            for( ulong i=0; i<prot.size(); i++ ) {
                regex_t _a(prot[i]); if( _a.test( _prot ) )
                { return prts[i]; }
            }
        }   return 8000;
    }
    
    /*─······································································─*/

    query_t query( const string_t& URL ){ return query::parse( search(URL) ); }
    
    /*─······································································─*/

    url_t parse( const string_t& URL ){ url_t data;
	if( !is_valid( URL ) ) return data;

        data.hostname = hostname( URL );
        data.protocol = protocol( URL );
        data.search   = search( URL );
        data.origin   = origin( URL );
	    data.query    = query( URL );
        data.pathname = path( URL );
        data.port     = port( URL );
        data.host     = host( URL );
        data.hash     = hash( URL );
        data.user     = user( URL );
        data.pass     = pass( URL );
        data.auth     = auth( URL );
        data.href     = URL;

        data.path = data.pathname + data.search;
        return data; 
    }
    
    /*─······································································─*/

    string_t format( url_t* URL=nullptr ){ string_t _url; 

        if( URL == nullptr ){ return _url; }

        if( !URL->href.empty() ){ return URL->href; }

        if( !URL->origin.empty() ){ _url += URL->origin; }
        else { 
            _url += URL->protocol + "//";
            _url += URL->auth + "@";
            _url += URL->host;
        }

        if( !URL->path.empty() ){ _url += URL->path; }
        else {
            _url += URL->pathname; 
            _url += URL->search;
        } 

        if( !URL->hash.empty() ){ _url += URL->hash; }
        return _url;
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_URL
#define NODEPP_URL

/*────────────────────────────────────────────────────────────────────────────*/

#include "encoder.h"
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
        return regex::test( URL, "^\\w+://([^.]+)", 1 );
    }

    /*.........................................................................*/

    string_t normalize ( string_t msg ) { string_t res = msg;
        while( regex::test( res, "%[a-z0-9]{2}", true ) ){
            auto data = regex::match( res, "%[a-z0-9]{2}", true );
            auto hex  = encoder::hex::set( data.slice(1) );
            auto y    = string_t( (char*)&hex,hex.size() );
            res = regex::replace_all( res, data, y );
        }   return res;
    }

    /*.........................................................................*/

    string_t unnormalize ( string_t msg ) { string_t res = msg;
        while( regex::test( res, "[^a-z0-9%]", true ) ){
            auto data = regex::match( res, "[^a-z0-9%]", true );
            auto hex  = encoder::hex::get( data[0] );
            res = regex::replace_all( res, data, "%"+hex );
        }   return res;
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
        regex_t _a("//\\w+:\\w+@");
        if( !is_valid(URL) || !_a.test( URL ) ) 
          { return null; } null = _a.match( URL );
            return null.slice( 2, -1 );
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
        string_t null; regex_t _a("/[^/?#]+");
        if ( !is_valid(URL) || !_a.test(URL) ){ return "/"; }
             null = _a.match_all( URL ).slice(1).join("");
	         return null.empty() ? "/" : null;
    }

    string_t host( const string_t& URL ){ 
        regex_t _a("(/|@)[^/#?]+");
        if(!is_valid(URL) ){ return nullptr; }
            auto data = _a.match( URL ).slice(1);
        if( regex::test( data, "@" ) )
             return regex::replace( data, "[^@]+@", "" );
        else return data;
    }

    string_t hostname( const string_t& URL ){ 
        string_t null = host(URL); regex_t _a("[^:]+");
        if( !is_valid(URL) || !_a.test( null ) ) 
          { return null; } return _a.match( null );
    }
    
    /*─······································································─*/

    uint port( const string_t& URL ){ 

        string_t _prot = protocol( URL );
        string_t _host = host( URL ); 
        regex_t  _a(":\\d+$");

        if( !_host.empty() && _a.test( _host ) ){
            return string::to_uint( _a.match( _host ).slice(1) );
        } elif( !_prot.empty() ) {
            for( ulong i=0; i<prot.size(); i++ ) {
             if( _prot.find( prot[i] ) != nullptr )
               { return prts[i]; }
            }
        }   
        
        return 8000;
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

    string_t format( url_t& obj ){ string_t _url; 

        if( !obj.href.empty() ){
            _url += obj.href;
        } elif( !obj.origin.empty() ){ 
            _url += obj.origin; 
        } else { 
            _url += obj.protocol + "//";
            _url += obj.auth + "@";
            _url += obj.host;
        }

        if( !obj.path.empty() ){ 
            _url += obj.path; 
        } else {
            _url += obj.pathname; 
        }

        if( !obj.search.empty() ){
            _url += obj.search;
        } else {
            _url += query::format( obj.query );
        }

        if( !obj.hash.empty() ){ _url += obj.hash; }

        return is_valid(_url) ? _url : nullptr;
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif

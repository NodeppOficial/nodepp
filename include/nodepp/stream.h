#ifndef NODEPP_STREAM
#define NODEPP_STREAM

/*────────────────────────────────────────────────────────────────────────────*/

#include "file.h"
#include "event.h"
#include "generator.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace stream {

    template< class T > void unpipe( const T& input ){ input.stop(); }
    
    /*─······································································─*/
    
    file_t pipe( const string_t& path, const string_t& mode ){
        auto inp = file_t( path, mode ); _stream_::pipe arg;
        process::poll::add( arg, inp );  return inp;
    }
    
    template< class... T >
    void pipe( const T&... inp ){ _stream_::pipe arg;
        process::poll::add( arg, inp... );
    }
    
    /*─······································································─*/
    
    file_t async( const string_t& path, const string_t& mode ){ return file_t( path, mode ); }
    
    /*─······································································─*/
    
    string_t sync( const string_t& path, const string_t& mode ){
        auto fp = file_t( path, mode ); string_t result;
        while ( !fp.is_available() ){ auto data = fp.read();
            if( !data.empty() ){ result += data; }
        }   return result;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_STREAM
#define NODEPP_STREAM

/*────────────────────────────────────────────────────────────────────────────*/

#include "file.h"
#include "event.h"
#include "generator.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace stream {

    template< class T > void unpipe( const T& input )
        { input.stop(); input.onUnpipe.emit(); }
    
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
    
    file_t line( const string_t& path, const string_t& mode ){
        auto inp = file_t( path, mode ); _stream_::line arg;
        process::poll::add( arg, inp );  return inp;
    }
    
    template< class... T >
    void line( const T&... inp ){ _stream_::line arg;
        process::poll::add( arg, inp... );
    }
    
    /*─······································································─*/
    
    template< class T >
    string_t await( const T& fp ){ string_t result;
        while ( fp.is_available() ){ auto data = fp.read();
           if (!data.empty() ){ result += data; }
        }   return result;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
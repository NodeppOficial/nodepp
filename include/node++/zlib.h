#ifndef NODEPP_ZLIB
#define NODEPP_ZLIB

/*────────────────────────────────────────────────────────────────────────────*/

#include <zlib.h>
#include <zconf.h>
#include "stream.h"
#include "generators.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace { template< class T >
string_t _inflate_( string_t input, T onData ){ z_stream stream;
    string_t output; char buffer[UNBFF_SIZE]; ulong size=0;

    stream.zfree     = Z_NULL;
    stream.zalloc    = Z_NULL;
    stream.opaque    = Z_NULL;
    stream.avail_in  = input.size();
    stream.next_in   = (Bytef*)input.c_str();

    if( onData( &stream ) != Z_OK )
        _Error("Failed to initialize zlib for decompression."); 
    
    do {

        stream.avail_out = UNBFF_SIZE;
        stream.next_out  = (Bytef*)buffer;
        int x = ::inflate( &stream, Z_FINISH );

        if(( size=UNBFF_SIZE-stream.avail_out )==0 ){
            output += (string_t){ buffer, size };
        }

        if( x == Z_STREAM_END ){ break; } else if( x < 0 ) {
            _Error( string::format("Compression failed: %s",stream.msg) ); 
        }
    
    } while ( stream.avail_in == 0 ); inflateEnd(&stream); return output;
}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace zlib { namespace rawinflate {

    string_t get( string_t input ){
        return nodepp::_inflate_( input, []( z_stream* stream ){
            return inflateInit( stream );
        });        
    }

    template< class T, class V >
    void pipe( const T& fileA, const V& fileB ){ _zlib_::inflate arg;
        process::poll::add( arg, fileA, fileB, []( z_stream* stream ){
            return inflateInit( stream );
        });
    }

    template< class T >
    void pipe( const T& fileA ){ _zlib_::inflate arg;
        process::poll::add( arg, fileA, []( z_stream* stream ){
            return inflateInit( stream );
        });
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace { template< class T >
string_t _deflate_( string_t input, T onData ){ z_stream stream;
    string_t output; char buffer[UNBFF_SIZE]; ulong size=0;

    stream.zfree     = Z_NULL;
    stream.zalloc    = Z_NULL;
    stream.opaque    = Z_NULL;
    stream.avail_in  = input.size();
    stream.next_in   = (Bytef*)input.c_str();

    if( onData( &stream ) != Z_OK )
        _Error("Failed to initialize zlib for compression."); 
    
    do {

        stream.avail_out = UNBFF_SIZE;
        stream.next_out  = (Bytef*)buffer;
        int x = ::deflate( &stream, Z_FINISH );

        if(( size=UNBFF_SIZE-stream.avail_out )==0 ){
            output += (string_t){ buffer, size };
        }

        if( x == Z_STREAM_END ){ break; } else if( x < 0 ) {
            _Error( string::format("Compression failed: %s",stream.msg) ); 
        }

    } while ( stream.avail_in == 0 ); deflateEnd(&stream); return output;
}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace zlib { namespace inflate {
    
    string_t get( string_t input ){
        return nodepp::_inflate_( input, []( z_stream* stream ){
            return inflateInit2( stream, -15 );
        });
    }

    template< class T, class V >
    void pipe( const T& fileA, const V& fileB ){ _zlib_::inflate arg;
        process::poll::add( arg, fileA, fileB, []( z_stream* stream ){
            return inflateInit2( stream, -15 );
        });
    }

    template< class T >
    void pipe( const T& fileA ){ _zlib_::inflate arg;
        process::poll::add( arg, fileA, []( z_stream* stream ){
            return inflateInit2( stream, -15 );
        });
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace zlib { namespace gunzip {

    string_t get( string_t input ){
        return nodepp::_inflate_( input, []( z_stream* stream ){
            return inflateInit2( stream, 15|32 );
        });        
    }

    template< class T, class V >
    void pipe( const T& fileA, const V& fileB ){ _zlib_::inflate arg;
        process::poll::add( arg, fileA, fileB, []( z_stream* stream ){
            return inflateInit2( stream, 15|32 );
        });
    }

    template< class T >
    void pipe( const T& fileA ){ _zlib_::inflate arg;
        process::poll::add( arg, fileA, []( z_stream* stream ){
            return inflateInit2( stream, 15|32 );
        });
    }
    
}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace zlib { namespace rawdeflate {

    string_t get( string_t input ){
        return nodepp::_deflate_( input, []( z_stream* stream ){
            return deflateInit( stream, Z_DEFAULT_COMPRESSION );
        });        
    }
    
    template< class T, class V >
    void pipe( const T& fileA, const V& fileB ){ _zlib_::inflate arg;
        process::poll::add( arg, fileA, fileB, []( z_stream* stream ){
            return deflateInit( stream, Z_DEFAULT_COMPRESSION );
        });
    }
    
    template< class T >
    void pipe( const T& fileA ){ _zlib_::deflate arg;
        process::poll::add( arg, fileA, []( z_stream* stream ){
            return deflateInit( stream, Z_DEFAULT_COMPRESSION );
        });
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace zlib { namespace deflate {

    string_t get( string_t input ){
        return nodepp::_deflate_( input, []( z_stream* stream ){
            return deflateInit2( stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15,8, Z_DEFAULT_STRATEGY );
        });        
    }

    template< class T, class V >
    void pipe( const T& fileA, const V& fileB ){ _zlib_::deflate arg;
        process::poll::add( arg, fileA, fileB, []( z_stream* stream ){
            return deflateInit2( stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY );
        });
    }
    
    /*─······································································─*/

    template< class T >
    void pipe( const T& fileA ){ _zlib_::deflate arg;
        process::poll::add( arg, fileA, []( z_stream* stream ){
            return deflateInit2( stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY );
        });
    } 
}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace zlib { namespace gzip {

    string_t get( string_t input ){
        return nodepp::_deflate_( input, []( z_stream* stream ){
            return deflateInit2( stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15|16, 8, Z_DEFAULT_STRATEGY );
        });        
    }

    template< class T, class V >
    void pipe( const T& fileA, const V& fileB ){ _zlib_::deflate arg;
        process::poll::add( arg, fileA, fileB, []( z_stream* stream ){
            return deflateInit2( stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15|16, 8, Z_DEFAULT_STRATEGY );
        });
    } 

    template< class T >
    void pipe( const T& fileA ){ _zlib_::deflate arg;
        process::poll::add( arg, fileA, []( z_stream* stream ){
            return deflateInit2( stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15|16, 8, Z_DEFAULT_STRATEGY );
        });
    } 

}}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif

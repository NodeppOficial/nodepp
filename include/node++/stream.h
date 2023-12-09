#ifndef NODEPP_STREAM
#define NODEPP_STREAM

/*────────────────────────────────────────────────────────────────────────────*/

#include "generators.h"
#include "event.h"
#include "file.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace stream {

    template< class T > void unpipe( T input ){ input.stop(); }
    
    /*─······································································─*/
    
    file_t pipe( const string_t& path, const string_t& mode ){
        auto inp = file_t( path, mode ); _stream_::pipe arg;
        process::poll::add( arg, inp );  return inp;
    }
    
    template< class T, class V >
    void pipe( const T& inp, const V& out ){
        _stream_::pipe arg;
        process::poll::add( arg, inp, out );
    }
    
    template< class T >
    void pipe( const T& inp ){
        _stream_::pipe arg;
        process::poll::add( arg, inp );
    }
    
    /*─······································································─*/
    
    file_t line( const string_t& path, const string_t& mode ){
        auto inp = file_t( path, mode ); _stream_::line arg;
        process::poll::add( arg, inp );  return inp;
    }
    
    template< class T, class V >
    void line( const T& inp, const V& out ){
        _stream_::line arg;
        process::poll::add( arg, inp, out );
    }
    
    template< class T >
    void line( const T& inp ){
        _stream_::line arg;
        process::poll::add( arg, inp );
    }
    
    /*─······································································─*/
    
    file_t async( const string_t& path, const string_t& mode ){
        auto fp = popen( (char*)path, (char*)mode ); if( fp == nullptr ) 
                       { $Error("such file or directory does not exist"); }
        auto fd = file_t( fileno( fp ) );
        fd.onClose([=](){ fclose( fp ); });
        return fd;
    }
    
    /*─······································································─*/
    
    string_t sync( const string_t& path, const string_t& mode ){
        auto fp = fopen( (char*)path, (char*)mode ); if( fp == nullptr ) 
                       { $Error("such file or directory does not exist"); }
        string_t result; ptr_t<char> buffer ( CHUNK_SIZE ); ulong c=0; 
        while ( !feof( fp ) ) {
            if((c=fread( &buffer, sizeof(char), CHUNK_SIZE, fp ))>0 )
              { result += (string_t){ &buffer, c }; } else { break; }
        }   fclose(fp); return result;
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
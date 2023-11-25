#ifndef NODEPP_STREAM
#define NODEPP_STREAM

/*────────────────────────────────────────────────────────────────────────────*/

#include "generators.h"
#include "event.h"
#include "file.h"
#include "exec.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace stream {

    file_t* readable_new( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return new file_t( path, "r", _size ); }
    file_t  readable( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return file_t( path, "r", _size ); }
    
    /*─······································································─*/

    file_t* writable_new( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return new file_t( path, "w", _size ); }
    file_t  writable( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return file_t( path, "w", _size ); }
    
    /*─······································································─*/

    file_t* tmp_new( const ulong& _size=CHUNK_SIZE ){ return new file_t( _size ); }
    file_t  tmp( const ulong& _size=CHUNK_SIZE ){ return file_t( _size ); }
    
#ifndef ARDUINO

    file_t* cout_new( const ulong& _size=CHUNK_SIZE ){ return new file_t( stdout, _size ); }
    file_t  cout( const ulong& _size=CHUNK_SIZE ){ return file_t( stdout, _size ); }
    
    /*─······································································─*/

    file_t* cerr_new( const ulong& _size=CHUNK_SIZE ){ return new file_t( stderr, _size ); }
    file_t  cerr( const ulong& _size=CHUNK_SIZE ){ return file_t( stderr, _size ); }
    
    /*─······································································─*/

    file_t* cin_new( const ulong& _size=CHUNK_SIZE ){ return new file_t( stdin, _size ); }
    file_t  cin( const ulong& _size=CHUNK_SIZE ){ return file_t( stdin, _size ); }

#endif

    template< class T > void unpipe( T input ){ input.stop(); }
    
    /*─······································································─*/
    
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

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
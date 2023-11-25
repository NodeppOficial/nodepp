#ifndef NODEPP_FILE
#define NODEPP_FILE

/*────────────────────────────────────────────────────────────────────────────*/

#include "generators.h"
#include <unistd.h>
#include <fcntl.h>
#include "event.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { 

/*────────────────────────────────────────────────────────────────────────────*/

namespace _file_ { struct str {

    ulong        range[2] = { 0, 0 };
    int          state = 0;
    ptr_t<char>  buffer;
    string_t     borrow;

    int          fd = -1;
    FILE*        fl = nullptr;
    FILE*        fp = nullptr;

};}

/*────────────────────────────────────────────────────────────────────────────*/

class file_t {
protected:

    ptr_t<_file_::str> obj = new _file_::str();
    
    int set_nonbloking_mode() const noexcept {
        static int flags = fcntl( obj->fd, F_GETFL, 0 );
        return fcntl( obj->fd, F_SETFL, flags | O_NONBLOCK );
    }
    
public:

    event_t<>          onUnpipe;
    event_t<>          onResume;
    event_t<except_t>  onError;
    event_t<>          onDrain;
    event_t<>          onClose;
    event_t<>          onStop;
    event_t<>          onOpen;
    event_t<>          onPipe;
    event_t<string_t>  onData;
    
    /*─······································································─*/
    
    virtual ~file_t() noexcept {
        if( obj.count() > 1 || obj->fd < 3 ){ return; } 
        if( obj->state == -2 ){ return; } force_close();
    }
    
    /*─······································································─*/

    file_t( const string_t& path, const string_t& mode, const ulong& _size=CHUNK_SIZE ) {
        obj->fl = fopen( (char*)path, (char*)mode  ); if( obj->fl == nullptr ) 
                  _Error("such file or directory does not exist");
        obj->fd = fileno( obj->fl ); set_nonbloking_mode(); set_buffer_size( _size );
    }

    file_t( FILE* stream, const ulong& _size=CHUNK_SIZE ) {
        if( stream == nullptr ){ _Error("such file or directory does not exist"); }
        obj->fl = stream; obj->fd = fileno( obj->fl ); 
        set_nonbloking_mode(); set_buffer_size( _size );
    }

    file_t( const int& df, const string_t& mode, const ulong& _size=CHUNK_SIZE ) {
        obj->fl = fdopen( df, (char*)mode ); if( obj->fl == nullptr ) 
                  _Error("such file or directory does not exist");
        obj->fd = df; set_nonbloking_mode(); set_buffer_size( _size );
    }

    file_t( const ulong& _size=CHUNK_SIZE ) noexcept {
        obj->fl = tmpfile(); obj->fd = fileno( obj->fl );
        set_nonbloking_mode(); set_buffer_size( _size );
    }

    /*─······································································─*/

    bool       is_closed() const noexcept { return obj->state < 0 || is_feof(); }
    virtual bool is_feof() const noexcept { return ::feof( get_fp() ); }
    bool    is_available() const noexcept { return obj->state == 0; }

    /*─······································································─*/
    
    void  reset() const noexcept { if(obj->state!=-2) { return; } flush(); rewind( get_fp() ); resume(); }
    void resume() const noexcept { if(obj->state== 0) { return; } obj->state= 0; onResume.emit(); }
    void  close() const noexcept { if(obj->state < 0) { return; } obj->state=-1; onDrain.emit(); }
    void   stop() const noexcept { if(obj->state==-3) { return; } obj->state=-3; onStop.emit(); }
    void   free() const noexcept { force_close(); } 
    
    /*─······································································─*/

    void set_range( const ulong(&x) [2] ) const noexcept { obj->range[0] = x[0]; obj->range[1] = x[1]; }
    void    set_range( ptr_t< ulong > x ) const noexcept { obj->range[0] = x[0]; obj->range[1] = x[1]; }
    void    set_range( ulong x, ulong y ) const noexcept { obj->range[0] = x;    obj->range[1] = y;    }
    
    /*─······································································─*/

    void      set_borrow( const string_t& brr ) const noexcept { obj->borrow = brr; }
    ulong     get_borrow_size() const noexcept { return obj->borrow.size(); }
    char*     get_borrow_data() const noexcept { return obj->borrow.data(); }
    void           del_borrow() const noexcept { obj->borrow.clear(); }
    string_t&      get_borrow() const noexcept { return obj->borrow; }
    
    /*─······································································─*/

    int    get_state() const noexcept { return obj == nullptr ? -1 : obj->state; }
    int       get_fd() const noexcept { return obj == nullptr ? -1 : obj->fd; }
    ulong* get_range() const noexcept { return obj->range; }
    
    /*─······································································─*/

    ulong   get_buffer_size() const noexcept { return obj->buffer.size(); }
    char*   get_buffer_data() const noexcept { return obj->buffer.data(); }
    ptr_t<char>& get_buffer() const noexcept { return obj->buffer; }
    
    /*─······································································─*/

    ulong size() const noexcept { ulong size=0; if( get_fp() == nullptr ) 
        { return size; } fpos_t pos; fgetpos( get_fp(), &pos );
        if( fseek( get_fp(), 0, SEEK_END ) == -1 ) return 0;
        size = ftell( get_fp() ); fsetpos( get_fp(), &pos );
    return size; }
    
    /*─······································································─*/

    FILE* get_fp() const noexcept {
        if( obj->fl != nullptr ){ return obj->fl; } 
        if( obj->fp != nullptr ){ return obj->fp; } return nullptr;
    }
    
    /*─······································································─*/

    virtual ulong set_buffer_size( ulong _size ) noexcept { 
        obj->buffer = ptr_t<char>( _size ); return _size;
    }
    
    /*─······································································─*/
    
    virtual void force_close() const noexcept {
        if( obj->state == -3 && obj.count() > 1 ){ resume(); return; }
        if( obj->state == -2 ){ return; } obj->state=-2;
        if( obj->fl != nullptr )  fclose( obj->fl );
        if( obj->fp != nullptr )  pclose( obj->fp );
        if( obj->fd != -1      ) ::close( obj->fd ); 
        close(); onClose.emit();
    }
    
    /*─······································································─*/

    ulong pos( ulong _pos ) const noexcept {
        auto   _npos =  fseek( get_fp(), _pos, SEEK_SET ); 
        return _npos == -1 ? 0 : _npos; 
    }

    ulong pos() const noexcept {
        auto   _pos =  ftell( get_fp() );
        return _pos == -1 ? 0 : _pos;
    }
    
    /*─······································································─*/

    void flush() const noexcept { 
        FILE* f = get_fp(); obj->buffer.fill(0); 
        if( f != nullptr ) fflush(f);
    }
    
    /*─······································································─*/

    bool is_blocked( int c ) const noexcept {
    if( c < 0 ){ return (
         errno == EWOULDBLOCK || errno == EINPROGRESS ||
         errno == ECONNRESET  || errno == EALREADY    ||
         errno == EAGAIN 
    ); } return 0; }
    
    /*─······································································─*/

    char     read_char() const noexcept { return read(1)[0]; }
    
    /*─······································································─*/

    string_t read_line() const noexcept {
        static auto gen = nodepp::_file_::line(); int c=0;
        while( gen( this ) == 1 )
             { process::next(); }
        return gen.y;
    }

    string_t read( ulong size=CHUNK_SIZE ) const noexcept {
        static auto gen = nodepp::_file_::read(); int c=0;
        while( gen( this, size ) == 1 )
             { process::next(); }
        return gen.y;
    }

    ulong write( string_t msg ) const noexcept {
        static auto gen = nodepp::_file_::write(); int c=0;
        while( gen( this, msg ) == 1 )
             { process::next(); }
        return gen.y;
    }
    
    /*─······································································─*/

    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } int c = 0;
        return is_blocked( c=::read( obj->fd, bf, sx ) ) ? -2 : c;
    }

    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } int c = 0;
        return is_blocked( c=::write( obj->fd, bf, sx ) ) ? -2 : c;
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
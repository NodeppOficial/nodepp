#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class file_t {
protected:

    struct _str_ {
        ulong        range[2] = { 0, 0 };
        OVERLAPPED   ov       = { 0 };
        int          state    =   0;
        ptr_t<char>  buffer;
        string_t     borrow;
        HANDLE       fd;
    };  ptr_t<_str_> obj = new _str_();
    
    /*─······································································─*/

    ptr_t<uint> get_fd_flag( const string_t& flag ){ 
        ptr_t<uint> fg ({ 0x00, FILE_SHARE_READ|FILE_SHARE_WRITE, 0x00, FILE_FLAG_OVERLAPPED });
             if( flag == "r"  ){ fg[0] |= GENERIC_READ;               fg[2] |= OPEN_EXISTING; }
        else if( flag == "w"  ){ fg[0] |= GENERIC_WRITE;              fg[2] |= CREATE_ALWAYS; }
        else if( flag == "a"  ){ fg[0] |= FILE_APPEND_DATA;           fg[2] |= OPEN_ALWAYS;   }
        else if( flag == "r+" ){ fg[0] |= GENERIC_READ|GENERIC_WRITE; fg[2] |= OPEN_EXISTING; }
        else if( flag == "w+" ){ fg[0] |= GENERIC_READ|GENERIC_WRITE; fg[2] |= OPEN_ALWAYS;   }
        else if( flag == "a+" ){ fg[0] |= FILE_APPEND_DATA;           fg[2] |= OPEN_EXISTING; }
        else                   { fg[0] |= GENERIC_READ|GENERIC_WRITE; fg[2] |= CREATE_ALWAYS; fg[2] |= FILE_ATTRIBUTE_TEMPORARY; }
        return  fg;
    }
    
    /*─······································································─*/

    virtual bool is_blocked( const bool& x, DWORD& c ) const noexcept {
        if ( x ){ return 0; } if ( os::error() != ERROR_IO_PENDING ){ return 0; }
        if ( GetOverlappedResult( obj->fd, &obj->ov, &c, 0 ) )
           { obj->ov.Offset += c; }
        return os::error() == ERROR_HANDLE_EOF ? 0 : c==0;
    }
    
public: file_t() noexcept {}

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
        if( obj.count() >  1 ){ return; } 
        if( obj->state == -2 ){ return; } force_close();
    }
    
    /*─······································································─*/

    file_t( const string_t& path, const string_t& mode, const ulong& _size=CHUNK_SIZE ){
        auto fg = get_fd_flag( mode ); obj->fd = CreateFileA( path.c_str(), fg[0], fg[1], NULL, fg[2], fg[3], NULL ); 
        if( obj->fd == INVALID_HANDLE_VALUE ) $Error("such file or directory does not exist");
            set_buffer_size( _size );
    }

    file_t( const HANDLE& fd, const ulong& _size=CHUNK_SIZE ) {
        if( fd == INVALID_HANDLE_VALUE ) $Error("such file or directory does not exist"); 
            obj->fd = fd; set_buffer_size( _size );
    }

    /*─······································································─*/

    bool       is_closed() const noexcept { return obj->state <  0 || is_feof(); }
    bool    is_available() const noexcept { return obj->state == 0; }
    virtual bool is_feof() const noexcept { return 0; }
    
    /*─······································································─*/

    void set_range( const ulong(&x) [2] ) const noexcept { obj->range[0] = x[0]; obj->range[1] = x[1]; }
    void    set_range( ptr_t< ulong > x ) const noexcept { obj->range[0] = x[0]; obj->range[1] = x[1]; }
    void    set_range( ulong x, ulong y ) const noexcept { obj->range[0] = x;    obj->range[1] = y;    }

    /*─······································································─*/
    
    void resume() const noexcept { if(obj->state== 0) { return; } obj->state= 0; onResume.emit(); }
    void  close() const noexcept { if(obj->state < 0) { return; } obj->state=-1; onDrain.emit();  }
    void   stop() const noexcept { if(obj->state==-3) { return; } obj->state=-3; onStop.emit();   }
    void  reset() const noexcept { if(obj->state!=-2) { return; } pos(0); flush(); resume();      }
    void  flush() const noexcept { obj->buffer.fill(0); }
    void   free() const noexcept { force_close(); } 
    
    /*─······································································─*/

    ulong* get_range() const noexcept { return obj == nullptr ? nullptr : obj->range; }
    int    get_state() const noexcept { return obj == nullptr ?      -1 : obj->state; }
    HANDLE    get_fd() const noexcept { return obj == nullptr ? nullptr : obj->fd; }
    
    /*─······································································─*/

    void   set_borrow( const string_t& brr ) const noexcept { obj->borrow = brr; }
    ulong  get_borrow_size() const noexcept { return obj->borrow.size(); }
    char*  get_borrow_data() const noexcept { return obj->borrow.data(); }
    void        del_borrow() const noexcept { obj->borrow.clear(); }
    string_t&   get_borrow() const noexcept { return obj->borrow; }
    
    /*─······································································─*/

    ulong   get_buffer_size() const noexcept { return obj->buffer.size(); }
    char*   get_buffer_data() const noexcept { return obj->buffer.data(); }
    ptr_t<char>& get_buffer() const noexcept { return obj->buffer; }
    
    /*─······································································─*/

    ulong pos( ulong _pos ) const noexcept { obj->ov.Offset = _pos; return _pos; }

    ulong size() const noexcept { LARGE_INTEGER size;
        while( !GetFileSizeEx(obj->fd,&size) ){ process::next(); }
        return size.QuadPart;
    }

    ulong pos() const noexcept { return obj->ov.Offset; }
    
    /*─······································································─*/

    virtual ulong set_buffer_size( ulong _size ) const noexcept { 
        obj->buffer = ptr_t<char>( _size ); return _size;
    }
    
    /*─······································································─*/
    
    virtual void force_close() const noexcept {
        if( obj->state == -3 && obj.count() > 1 ){ resume(); return; }
        if( obj->state == -2 ){ return; } obj->state = -2;
        CloseHandle( obj->fd ); close(); onClose.emit();
    }
    
    /*─······································································─*/

    char read_char() const noexcept { return read(1)[0]; }
    
    /*─······································································─*/

    string_t read_line() const noexcept {
        static auto gen = nodepp::_file_::line();
        while( gen( this ) == 1 )
             { process::next(); }
        return gen.y;
    }

    string_t read( ulong size=CHUNK_SIZE ) const noexcept {
        static auto gen = nodepp::_file_::read();
        while( gen( this, size ) == 1 )
             { process::next(); }
        return gen.y;
    }

    ulong write( const string_t& msg ) const noexcept {
        static auto gen = nodepp::_file_::write();
        while( gen( this, msg ) == 1 )
             { process::next(); }
        return gen.y;
    }
    
    /*─······································································─*/

    virtual int _read( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } DWORD c = 0;
        return is_blocked( ReadFile( obj->fd, bf, sx, &c, &obj->ov ), c ) ? -2 : c;
    }

    virtual int _write( char* bf, const ulong& sx ) const noexcept {
        if( is_closed() ){ return -1; } DWORD c = 0;
        return is_blocked( WriteFile( obj->fd, bf, sx, &c, &obj->ov ), c ) ? -2 : c;
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/
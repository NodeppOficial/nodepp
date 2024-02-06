#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <unistd.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class cluster_t { 
protected:

    struct _str_ {
        int          fd;
        int    state =0;
        file_t writable;
        file_t readable;
    };  ptr_t<_str_> obj;

public:

    event_t<>          onResume;
    event_t<except_t>  onError;
    event_t<>          onClose;
    event_t<>          onStop;
    event_t<>          onExit;
    event_t<>          onOpen;
    event_t<string_t>  onData;
    
    virtual ~cluster_t() noexcept { 
        if( obj.count() > 1 ){ return; } 
        if( obj->state == 0 ){ return; }
        //  force_close();
    }

    template< class... T >
    cluster_t( const T&... args ) : obj( new _str_() ) {

        if( process::is_child() ){ 
            int fd[2] = { 0, 0 };
            string_t ch = process::env::get("CHILD");
            string::parse( ch.data(), "%d|%d", &fd[0], &fd[1] );
            obj->writable = { fd[0] }; obj->readable = { fd[1] };
            return;
        }

        int fda[2]; ::pipe( fda ); 
        int fdb[2]; ::pipe( fdb ); obj->fd = ::fork();

        if( obj->fd == 0 ){ // Child process
            auto cm = process::args[0];::close(fdb[0]);::close(fda[1]);
            auto ch = string::format( "?CHILD=%d|%d", fda[0], fdb[0] );
            ::execl( cm.data(), cm.data(), args..., ch.data( ), NULL );
            process::error("while running new process"); process::exit(1);
        } elif ( obj->fd > 0 ) { // Parent process
            obj->writable = { fda[1] }; ::close( fda[0] );
            obj->readable = { fdb[0] }; ::close( fdb[1] );
        } else {
            ::close( fda[0] ); ::close( fda[1] );
            ::close( fdb[0] ); ::close( fdb[1] );
        }

    }
    
    /*─······································································─*/

    bool is_available() const noexcept {
        return obj->readable.is_available() &&
               obj->writable.is_available()  ;
    }

    bool is_closed() const noexcept {
        return obj->readable.is_closed() ||
               obj->writable.is_closed()  ;
    }

    bool is_alive() const noexcept {
        return ::kill( obj->fd, 0 ) == 0;
    }

    int get_fd() const noexcept {
        return obj->fd;
    }

    /*─······································································─*/

    virtual void force_close() const noexcept {
        if( obj->state == -3 && obj.count() > 1 ){ resume(); return; }
        if( obj->state == -2 ){ return; } obj->state = -2;
        obj->readable.close(); obj->writable.close();
        close(); kill(); onClose.emit();
    }

    /*─······································································─*/
    
    void resume() const noexcept { if(obj->state== 0) { return; } obj->state= 0; onResume.emit(); }
    void  close() const noexcept { if(obj->state < 0) { return; } obj->state=-1; onExit.emit(); }
    void   stop() const noexcept { if(obj->state==-3) { return; } obj->state=-3; onStop.emit(); }
    void  flush() const noexcept { writable().flush(); readable().flush(); }
    void   kill() const noexcept { ::kill( obj->fd, SIGKILL ); }
    void   free() const noexcept { force_close(); }
    
    /*─······································································─*/

    void pipe() const noexcept { 
        if( obj->state == 1 ){ return; } obj->state = 1; onOpen.emit();
            ptr_t<_file_::read> _read = new _file_::read;
            auto inp = type::bind( this );

        process::task::add([=](){
            if(!inp->is_available() ){ inp->close(); return -1; }
            if((*_read)(&inp->readable())==1 )     { return  1; }
            if(  _read->c <= 0  )                  { return  1; }
            inp->onData.emit(_read->y);              return  1;
        });

        onExit([=](){ inp->free(); });
    }
    
    /*─······································································─*/

    template< class... T >
    string_t read( const T&... args ) const noexcept { 
        return obj->readable.read( args... ); 
    }

    template< class... T >
    int write( const T&... args ) const noexcept { 
        return obj->writable.write( args... ); 
    }
    
    /*─······································································─*/

    template< class... T >
    string_t _read( const T&... args ) const noexcept { 
        return obj->readable._read( args... ); 
    }

    template< class... T >
    int _write( const T&... args ) const noexcept { 
        return obj->writable._write( args... ); 
    }
    
    /*─······································································─*/
    
    file_t& writable() const noexcept { return obj->writable; }

    file_t& readable() const noexcept { return obj->readable; }

    /*─······································································─*/

    bool  is_child() const noexcept { return !process::env::get("CHILD").empty(); }

    bool is_parent() const noexcept { return  process::env::get("CHILD").empty(); }

};}

/*────────────────────────────────────────────────────────────────────────────*/
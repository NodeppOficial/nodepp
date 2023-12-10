#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <unistd.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class popen_t { 
protected:

    struct _str_ {
        int    pid;
        int    state=0;
        file_t writable;
        file_t readable;
    };  ptr_t<_str_> obj;

public: popen_t() noexcept {}
    
    /*─······································································─*/

    virtual ~popen_t() noexcept {
        if( obj.count() > 1 ){ return; } 
        if( obj->state == 0 ){ return; }
    //  _init_( obj->writable, obj->readable );
    }

    template< class... T >
    popen_t( const string_t& cmd, T... args ) : obj( new _str_() ) {

        if( process::is_child() ){
            obj->writable = { STDOUT_FILENO };
            obj->readable = { STDIN_FILENO  }; return;
        }

        int fda[2]; ::pipe( fda ); 
        int fdb[2]; ::pipe( fdb ); obj->pid = ::fork();

        if( obj->pid == 0 ){ // Child process
            ::dup2( fdb[1], STDOUT_FILENO ); ::close( fdb[0] );
            ::dup2( fda[0], STDIN_FILENO  ); ::close( fda[1] );
            ::execl( "/bin/bash", "bash", "-c", cmd.data(), args..., NULL );
            console::error("while running new process"); process::exit( 1 );
        } else if( obj->pid > 0 ) { // Parent process
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
               obj->writable.is_available() ;
    }

    bool is_closed() const noexcept {
        return obj->readable.is_closed() ||
               obj->writable.is_closed() ;
    }

    void close() const noexcept { 
        if( obj->state<0 ) { return; }
        obj->readable.close();
        obj->writable.close();
        obj->state=-1;
    }
    
    /*─······································································─*/

    ptr_t<file_t> pipe() const noexcept { return {{ obj->readable, obj->writable }}; }

    file_t& writable() const noexcept { return obj->writable; }

    file_t& readable() const noexcept { return obj->readable; }

    /*─······································································─*/

    bool  is_child() const noexcept { return !process::env::get("CHILD").empty(); }

    bool is_parent() const noexcept { return  process::env::get("CHILD").empty(); }

};}

/*────────────────────────────────────────────────────────────────────────────*/
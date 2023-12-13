#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <unistd.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class cluster_t { 
protected:

    struct _str_ {
        int    pid;
        int    state=0;
        file_t writable;
        file_t readable;
    };  ptr_t<_str_> obj;

public:
    
    virtual ~cluster_t() noexcept {
        if( obj.count() > 1 ){ return; } 
        if( obj->state == 0 ){ return; }
    //  _init_( obj->writable, obj->readable );
    }

    template< class... T >
    cluster_t( T... args ) : obj( new _str_() ) {

        if( process::is_child() ){ 
            int fd[2] = { 0, 0 };
            string_t ch = process::env::get("CHILD");
            string::parse( ch.data(), "%i@%i", &fd[0], &fd[1] );
            obj->writable = { fd[0] }; obj->readable = { fd[1] };
            return;
        }

        int fda[2]; ::pipe( fda ); 
        int fdb[2]; ::pipe( fdb ); int pid = ::fork();

        if( pid == 0 ){ // Child process
            auto cm = process::args[0];::close(fdb[0]);::close(fda[1]);
            auto ch = string::format( "?CHILD=%d@%d", fda[0], fdb[0] );
            ::execl( cm.data(), cm.data(), args..., ch.data(), NULL );
            console::error("while running new process"); process::exit(1);
        } else if( pid > 0 ) { // Parent process
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

    string_t read( const ulong& size=CHUNK_SIZE ) const noexcept { return obj->readable.read(size); }

    int write( const string_t& msg ) const noexcept { return obj->writable.write(msg); }
    
    /*─······································································─*/

    ptr_t<file_t> pipe() const noexcept { return {{ obj->readable, obj->writable }}; }

    file_t& writable() const noexcept { return obj->writable; }

    file_t& readable() const noexcept { return obj->readable; }

    /*─······································································─*/

    bool  is_child() const noexcept { return !process::env::get("CHILD").empty(); }

    bool is_parent() const noexcept { return  process::env::get("CHILD").empty(); }

};}

/*────────────────────────────────────────────────────────────────────────────*/
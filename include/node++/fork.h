#ifndef NODEPP_FORK
#define NODEPP_FORK

/*────────────────────────────────────────────────────────────────────────────*/

#include "regex.h"
#include "event.h"
#include "stream.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

class fork_t { 
protected:

    void _init_( file_t& out, file_t& inp ) {
        ptr_t<fork_t> self = new fork_t( *this );
        ptr_t<_file_::read> _read = new _file_::read;

        process::task::add([=](){
            if(!inp.is_available() )   { inp.close(); return -1; }
            if((*_read)(&inp)==1 )     { return 1; } 
            if(  _read->c <= 0  )      { return 1; }
            self->onData.emit(_read->y); return 1;
        });

        inp.onPipe([=](){ self->onPipe.emit(); });
        inp.onClose([=](){ self->onClose.emit(); });
        inp.onUnpipe([=](){ self->onUnpipe.emit(); });
        
        inp.onDrain([=](){ 
            self->onDrain.emit(); out.free(); inp.free(); 
            if( process::is_child() ){ process::exit(1); }
        });
        
        out.onError([=]( except_t err ){ self->onError.emit( err ); });
        inp.onError([=]( except_t err ){ self->onError.emit( err ); });

    }

    file_t writable, readable;

public: fork_t() noexcept {}

    event_t<>          onUnpipe;
    event_t<except_t>  onError;
    event_t<>          onDrain;
    event_t<>          onClose;
    event_t<>          onPipe;
    event_t<string_t>  onData;
    
    /*─······································································─*/

    template< class... T >
    fork_t( const string_t& cmd, T... args ){

        if( strncmp( process::args[process::args.last()-1].data(), "?CHILD", 6 ) == 0 ){  

            for( auto x=process::args.size(); x--; ){
             if( !regex::test(process::args[x],"^\\?") ){ continue; }
                auto data = regex::match_all(process::args[x],"[^?=]+");
                            process::env::set( data[0], data[1] ); 
                            process::args.erase(x);
            }

            string_t frm = process::env::get("CHILD");
            int fdw, fdr; string::parse( frm, "%d,%d", &fdw, &fdr );
            writable = { fdr, "w" }; readable = { fdw, "r" };
            _init_( writable, readable ); return; 
        }

        int fda[2]; ::pipe( fda ); 
        int fdb[2]; ::pipe( fdb ); 
        pid_t pid = ::fork();

        if( pid == 0 ){ // Child process
            string_t _fd = string::format( "?CHILD=%d,%d", fda[0], fdb[1] );
            execl( cmd.data(), cmd.data(), args..., _fd.data(), NULL );
            process::exit(1); return;
        } else { // Parent process
            writable = { fda[1], "w" }; readable = { fdb[0], "r" }; 
            _init_( writable, readable ); return; 
        }

    }
    
    /*─······································································─*/

    string_t  read() const noexcept { return readable.read(); }

    int      write( string_t msg ) const noexcept { return writable.write( msg ); }
    
    /*─······································································─*/

    bool  is_child() const noexcept { return !process::env::get("CHILD").empty(); }

    bool is_parent() const noexcept { return  process::env::get("CHILD").empty(); }

};

/*────────────────────────────────────────────────────────────────────────────*/

class popen_t : public file_t { public:

    popen_t( const string_t& path, const string_t& mode="r", const ulong& _size=CHUNK_SIZE ){
        obj->fp =  popen( (char*)path, (char*)mode  ); if( obj->fp == nullptr ) 
                        { _Error("such file or directory does not exist"); }
        obj->fd = fileno( obj->fp ); obj->buffer = ptr_t<char>( _size ); 
        set_nonbloking_mode();
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
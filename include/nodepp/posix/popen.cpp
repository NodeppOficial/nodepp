#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <unistd.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class popen_t { 
protected:

    struct _str_ {
        int           fd;
        int     state =0;
        file_t     stdin;
        file_t    stderr;
        file_t    stdout;
    };  ptr_t<_str_> obj;

    template< class T >
    void _init_( const string_t& path, T& arg, T& env ) {

        int fda[2]; ::pipe( fda ); 
        int fdb[2]; ::pipe( fdb );
        int fdc[2]; ::pipe( fdc ); obj->fd = ::fork();

        if( obj->fd == 0 ){ // Child process
            ::dup2( fda[0], STDIN_FILENO  ); ::close( fda[1] );
            ::dup2( fdb[1], STDOUT_FILENO ); ::close( fdb[0] ); arg.push( nullptr );
            ::dup2( fdc[1], STDERR_FILENO ); ::close( fdc[0] ); env.push( nullptr );
            ::execvpe( path.c_str(), (char*const*) arg.data(), (char*const*) env.data() );
            process::error("while spawning new popen"); process::exit(1);
        } elif ( obj->fd > 0 ) { // Parent process
            obj->stdin  = { fda[1] }; ::close( fda[0] );
            obj->stdout = { fdb[0] }; ::close( fdb[1] );
            obj->stderr = { fdc[0] }; ::close( fdc[1] );
        } else {
            ::close( fda[0] ); ::close( fda[1] );
            ::close( fdb[0] ); ::close( fdb[1] );
            ::close( fdc[0] ); ::close( fdc[1] );
        }

    }

public:

    event_t<>          onResume;
    event_t<except_t>  onError;
    event_t<>          onClose;
    event_t<>          onStop;
    event_t<>          onExit;
    event_t<>          onOpen;

    event_t<string_t>  onData;
    event_t<string_t>  onDout;
    event_t<string_t>  onDerr;

    virtual ~popen_t() noexcept {
        if( obj.count() > 1 ){ return; } 
        if( obj->state == 0 ){ return; }
        //  force_close();
    }

    template< class... T >
    popen_t( const string_t& path, const initializer_t<string_t>& args ) : obj( new _str_() ) {
        array_t<const char*> arg; array_t<const char*> env; bool y=0;

        for ( auto x : args ) {
           if ( x != nullptr && !y ) arg.push( x.c_str() );
         elif ( x != nullptr &&  y ) env.push( x.c_str() );
         else   y =! y;
        }
        
        _init_( path, arg, env );
    }

    /*─······································································─*/

    bool is_alive() const noexcept { return ::kill( obj->fd, 0 ) == 0; }

    int get_fd()    const noexcept { return obj->fd; }

    /*─······································································─*/

    virtual void force_close() const noexcept {
        if( obj->state == -3 && obj.count() > 1 ){ resume(); return; }
        if( obj->state == -2 ){ return; } obj->state = -2;
        obj->stdout.close(); 
        obj->stderr.close(); 
        obj->stdin .close(); 
        close(); kill(); onClose.emit();
    }

    /*─······································································─*/
    
    void resume() const noexcept { if(obj->state== 0) { return; } obj->state= 0; onResume.emit(); }
    void  close() const noexcept { if(obj->state < 0) { return; } obj->state=-1; onExit.emit(); }
    void   stop() const noexcept { if(obj->state==-3) { return; } obj->state=-3; onStop.emit(); }
    void  flush() const noexcept { stdin().flush(); stdout().flush(); stderr().flush(); }
    void   kill() const noexcept { ::kill( obj->fd, SIGKILL ); }
    void   free() const noexcept { force_close(); }
    
    /*─······································································─*/

    void pipe() const noexcept { 
        if( obj->state == 1 ){ return; } obj->state = 1; onOpen.emit();
            ptr_t<_file_::read> _read1 = new _file_::read;
            ptr_t<_file_::read> _read2 = new _file_::read;
            auto inp = type::bind( this );
            onExit([=](){ inp->free(); });

        process::task::add([=](){
            if(!inp->stdout().is_available() ){ inp->close(); return -1; }
            if((*_read1)(&inp->stdout())==1 ) { return  1; }
            if(  _read1->c <= 0  )            { return  1; }
            inp->onData.emit(_read1->y);    
            inp->onDout.emit(_read1->y);        return  1;
        });

        process::task::add([=](){
            if(!inp->stderr().is_available() ){ inp->close(); return -1; }
            if((*_read2)(&inp->stderr())==1 ) { return  1; }
            if(  _read2->c <= 0  )            { return  1; }
            inp->onData.emit(_read2->y);   
            inp->onDerr.emit(_read2->y);        return  1;
        });

    }
    
    /*─······································································─*/

    template< class... T >
    int write( const T&... args )     const noexcept { return stdin().write( args... ); }

    template< class... T >
    string_t read( const T&... args ) const noexcept { return stdin().read( args... ); }
    
    /*─······································································─*/

    template< class... T >
    int _write( const T&... args ) const noexcept { return stdin()._write( args... ); }

    template< class... T >
    int _read( const T&... args )  const noexcept { return stdout()._read( args... ); }
    
    /*─······································································─*/

    file_t& stderr() const noexcept { return obj->stderr; }
    file_t& stdout() const noexcept { return obj->stdout; }
    file_t&  stdin() const noexcept { return obj->stdin;  }

};}

/*────────────────────────────────────────────────────────────────────────────*/
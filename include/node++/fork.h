#ifndef NODEPP_FORK
#define NODEPP_FORK

/*────────────────────────────────────────────────────────────────────────────*/

#include "event.h"
#include "file.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

class fork_t : public file_t { public:
protected:

public: fork_t(){}

    fork_t( const string_t& name ){}

};

/*────────────────────────────────────────────────────────────────────────────*/

class popen_t : public file_t { public:

    popen_t( const string_t& path, const string_t& mode="r", const ulong& _size=CHUNK_SIZE ) {
        obj->fp =  popen( (char*)path, (char*)mode  ); if( obj->fp == nullptr ) 
                        { _Error("such file or directory does not exist"); }
        obj->fd = fileno( obj->fp ); obj->buffer = ptr_t<char>( _size ); 
        set_nonbloking_mode();
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

class system_t { public: int status = 0;
    system_t( const string_t& cmd ) noexcept { status = system( cmd.c_str() ); }
    explicit operator int(void){ return status; }
};

/*────────────────────────────────────────────────────────────────────────────*/

namespace process {
    template< class... T > system_t spawn( T... args ){ return system_t(args...); }
    template< class... T > popen_t   exec( T... args ){ return popen_t(args...); }
    template< class... T > fork_t    fork( T... args ){ return fork_t(args...); }
}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
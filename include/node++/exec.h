#ifndef NODEPP_EXEC
#define NODEPP_EXEC

/*────────────────────────────────────────────────────────────────────────────*/

#include "event.h"
#include "file.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

class exec_t : public file_t { public:

    exec_t( const string_t& path, const string_t& mode="r", const ulong& _size=CHUNK_SIZE ) {
        obj->fp = popen( (char*)path, (char*)mode  ); if( obj->fp == nullptr ) 
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

template< class... T > 
string_t exec_sync( T... args ){ string_t output; exec_t p( args... );
    while( !p.is_closed() ){ output += p.read(); } return output;
}

/*────────────────────────────────────────────────────────────────────────────*/

namespace process {
    template< class... T > exec_t   exec_sync( T... args ){ return nodepp::exec_sync(args...); }
    template< class... T > system_t spawn( T... args ){ return system_t(args...); }
    template< class... T > exec_t   exec( T... args ){ return exec_t(args...); }
}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
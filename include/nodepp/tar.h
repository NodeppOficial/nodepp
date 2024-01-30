#ifndef NODEPP_TAR
#define NODEPP_TAR

/*────────────────────────────────────────────────────────────────────────────*/

#include "stream.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { 

/*────────────────────────────────────────────────────────────────────────────*/

struct tar_header_t {
    char linkname[100] = "\0";
    char name[100]     = "\0";
    char mode[8]       = "0755";
    char uid[8]        = "1000";
    char gid[8]        = "1000";
    char mtime[12]     = "\0";
    char size[12]      = "\0";
    char typeflag      = '\0';
    char chksum[8]     = "\0";
    char magic[6]      = "ustar";
    char gname[32]     = "group";
    char uname[32]     = "user";
    char version[2]    = "\0";
    char devmajor[8]   = "\0";
    char devminor[8]   = "\0";
    char prefix[155]   = "\0";
};

/*────────────────────────────────────────────────────────────────────────────*/

class tar_t : public file_t {     
public: tar_t() noexcept : file_t(){} 

    template< class... T > tar_t( const T&... args ) noexcept : file_t( args... ) {}

    void write_header( const string_t& name, ulong size, ulong time=0, const string_t& chk="" ) const noexcept { 
        tar_header_t head; memcpy( head.name, name.data(), 100 );
        memcpy( head.mtime, string::to_string(time).data(), 12 );
        memcpy( head.size,  string::to_string(size).data(), 12 );
        memcpy( head.chksum, string::to_string(chk).data(), 8  );
        string_t data ( (char*) &head, sizeof(head) );
        this->write( data );
    }

    template < class T >
    ulong search_file( T func ) const noexcept { 
        tar_header_t head; while( pos() != size() ){ 
            head = read_header(); if( !func(head) ){ 
                ulong siz = pos() - borrow->size() + string::to_ulong( head.size ); 
                pos( siz ); borrow->clear();
            } else { return pos(); }
        }   return 0;
    }

    template < class T >
    int exists_file( T func ) const noexcept { 
        tar_header_t head; while( pos() != size() ){ 
            head = read_header(); if( !func(head) ){
                ulong siz = pos() - borrow->size() + string::to_ulong( head.size ); 
                pos( siz ); borrow->clear();
            } else { return 1; }
        }   return -1;
    }

    tar_header_t read_header() const noexcept { tar_header_t head; 
        string_t data = read_chunk( sizeof(head) );
        if( data.empty() ){ (*state)=-2; return head; } 
        return *((tar_header_t*)((void*)data.c_str()));
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

namespace tar {



}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
#ifndef NODEPP_EXEC
#define NODEPP_EXEC

/*────────────────────────────────────────────────────────────────────────────*/

#include "stream.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace exec {
    
    /*─······································································─*/
    
    file_t async( const string_t& cmd  ){
        auto fp = popen( (char*)cmd, "r" ); if( fp == nullptr ) 
                       { $Error("command does not run"); }
        auto fd = file_t( fileno( fp ) );
        fd.onClose([=](){ pclose( fp ); });
        return fd;
    }
    
    /*─······································································─*/
    
    string_t sync( const string_t& cmd  ){
        auto fp = popen( (char*)cmd, "r" ); if( fp == nullptr ) 
                       { $Error("command does not run"); }
        string_t result; ptr_t<char> buffer ( CHUNK_SIZE ); ulong c=0;
        while ( !feof( fp ) ) {
            if((c=fread( &buffer, sizeof(char), CHUNK_SIZE, fp ))!=0 )
              { result += (string_t){ &buffer, c }; } else { break; }
        }   pclose(fp); return result;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
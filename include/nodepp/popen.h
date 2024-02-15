#ifndef NODEPP_POPEN
#define NODEPP_POPEN

/*────────────────────────────────────────────────────────────────────────────*/

#if   _KERNEL == NODEPP_KERNEL_WINDOWS
    #include "fs.h"
    #include "initializer.h"
    #include "windows/popen.cpp"
#elif _KERNEL == NODEPP_KERNEL_POSIX
    #include "fs.h"
    #include "initializer.h"
    #include "posix/popen.cpp"
#else
    #error "This OS Does not support popen.h"
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace popen {

    template< class... T >
    popen_t async( const string_t& path, const initializer_t<string_t>& args ){ 
    popen_t pid  ( path, args ); pid.pipe(); return pid;
    }

    popen_t async( const string_t& path ){
     return async( path, { path } );
    }
    
    /*─······································································─*/
    
    template< class... T >
    string_t await( const string_t& path, const initializer_t<string_t>& args ){
        string_t result; auto fp = popen_t( path, args ); 
        while ( fp.std_output().is_available() ){ 
           auto data = fp.std_output().read();
           if (!data.empty() ){ result += data; }
        }       return result;
    }

    string_t await( const string_t& path ){
      return await( path, { path } );
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
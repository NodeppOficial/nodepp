#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace { 

    int CLEAR(){ LPCH environmentStrings = GetEnvironmentStrings();
        for( LPCTSTR currentVariable = environmentStrings; *currentVariable; currentVariable += lstrlen(currentVariable) + 1 ){
            SetEnvironmentVariableA( currentVariable, nullptr );
        }   FreeEnvironmentStrings( environmentStrings ); return 0;
    }

    string_t GET( const string_t& name ){ ptr_t<char> buffer ( UNBFF_SIZE );
        auto x = GetEnvironmentVariableA( name.c_str(), &buffer, buffer.size() );
        return (string_t){ &buffer, (ulong) x };
    } 
    
    int SET( const string_t& name, const string_t& value ){ 
        return SetEnvironmentVariableA( name.c_str(), value.c_str() );
    }

    int DEL( const string_t& name ){
        return SetEnvironmentVariableA( name.c_str(), nullptr ); 
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    namespace env {

        int set( const string_t& name, const string_t& value ){ return nodepp::SET( name, value ); }

        string_t get( const string_t& name ){ return nodepp::GET( name ); }

        int remove( const string_t& name ){ return nodepp::DEL( name );  } 

        int clear(){ return nodepp::CLEAR(); }

        int init( const string_t& path ){ try {
                
            FILE* v = fopen( path.c_str(), "r" ); 
            string_t s; bool nr = 0; bool pr = 0;
            
            array_t<string_t> env ( 2, "" ); 

            function_t<void> lb([&](){ nodepp::SET( env[0], env[1] ); });

            while( !feof(v) ){ int c = fgetc( v );

                if( c == 39 || c == 34 ){ pr=!pr; continue; }

                if( c == 32 && !pr ){ continue; }
                if( c == 35 ){ nr = true; continue; }
                if( c == 61 ){ env[0]=s; s.clear(); continue; } 

                else if( c == -1 ){ env[1]=s; s.clear(); lb(); nr=0; pr=0; break; }
                else if((c == 10 || c == 59 ) && !pr ){ env[1]=s; s.clear(); lb(); nr=0; continue; }

                if( !nr ) s.push(c);
            }   fclose(v); return  1;
            } catch(...) { return -1; }
        };

    }
    
    /*─······································································─*/

    template< class... T >
    int     spawn( const T&... args ){ return ::system(args...); }

    void     exit( int err=0 ){ ::exit(err); }

    void    abort(){ ::abort(); }
    
    /*─······································································─*/

    bool  is_child(){ return !env::get("CHILD").empty(); }

    bool is_parent(){ return  env::get("CHILD").empty(); }

    string_t  home(){ return env::get("USERPROFILE"); }
    
    string_t shell(){ return env::get("COMSPEC"); }
    
}}

/*────────────────────────────────────────────────────────────────────────────*/
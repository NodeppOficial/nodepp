/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#pragma once
#include <unistd.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace { 
    
    int SET( const string_t& name, const string_t& value ){ return setenv( name.c_str(), value.c_str(), 1 ); }

    string_t GET( const string_t& name ){ return getenv( name.c_str() ); } 

    int DEL( const string_t& name ){ return unsetenv( name.c_str() ); }

  //int CLEAR(){ return clearenv(); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    namespace env {

        int set( const string_t& name, const string_t& value ){ return nodepp::SET( name, value ); }

        string_t get( const string_t& name ){ return nodepp::GET( name ); }

        int remove( const string_t& name ){ return nodepp::DEL( name );  } 

      //int clear(){ return nodepp::CLEAR(); }

        int init( const string_t& path ){
                
            FILE* v = fopen( path.c_str(), "r" ); 
            string_t s; bool nr = 0; bool pr = 0;

            if( v == nullptr ){ 
               _ERROR( "such file of directory not found" );
                return -1; 
            }
            
            array_t<string_t> env ( 2 ); 
            function_t<void> lb([&](){ 
                if( env[0].empty() ){ return; }
                if( env[1].empty() ){ return; }
                nodepp::SET( env[0], env[1] ); 
            });

            while( !feof(v) ){ int c = fgetc( v );

                if( c=='"' ){ pr=!pr;continue; }
                if( c==' ' && !pr ){ continue; }

                if( c=='#' && !pr ){ nr=1; continue; }
                if( c==';' && !pr ){ nr=1; continue; }

                  if( c=='=' && !pr && !nr ){ env[0]=s; s.clear(); continue; } 
                elif( c==-1 )               { env[1]=s; s.clear(); lb(); nr=0; break; }
                elif( c=='\n'&& !pr )       { env[1]=s; s.clear(); lb(); nr=0; continue; }

                if( !nr ) s.push(c);
            }   fclose(v); return  1;
        };

    }
    
    /*─······································································─*/

    bool  is_child(){ return !env::get("CHILD").empty(); }

    bool is_parent(){ return  env::get("CHILD").empty(); }
    
    string_t shell(){ return env::get("SHELL"); }

    string_t  home(){ return env::get("HOME"); }
    
}}

/*────────────────────────────────────────────────────────────────────────────*/
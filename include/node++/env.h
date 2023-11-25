#ifndef NODEPP_ENV
#define NODEPP_ENV

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace { 
    int SET( string_t name, string_t value ){ return setenv( name.c_str(), value.c_str(), 1 ); }
    string_t GET( string_t name ){ return getenv( name.c_str() ); } 
    int DEL( string_t name ){ return unsetenv( name.c_str() ); }
    int CLEAR(){ return clearenv(); }
}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    void exit( int err=0 ){ ::exit(err); }
    
    /*─······································································─*/

    namespace env {

        int set( string_t name, string_t value ){ return nodepp::SET( name, value ); }

        string_t get( string_t name ){ return nodepp::GET( name ); }

        int remove( string_t name ){ return nodepp::DEL( name );  } 

        int clear(){ return nodepp::CLEAR(); }

        int init( string_t path ){ try {
                
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
    
    string_t shell(){ return env::get("SHELL"); }

    string_t home(){ return env::get("HOME"); }
    
}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
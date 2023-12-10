#pragma once
#define TMP_FILE string::format("%s%s",os::tmp().c_str(),"nodepp_XXXXXX").data()

/*────────────────────────────────────────────────────────────────────────────*/

#include <sys/stat.h>
#include <dirent.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace fs {

    file_t  readable( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return file_t( path, "r", _size ); }

    file_t  writable( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return file_t( path, "w", _size ); }

    file_t  cout( const ulong& _size=CHUNK_SIZE ){ return file_t( STDOUT_FILENO, _size ); }
    
    file_t  cerr( const ulong& _size=CHUNK_SIZE ){ return file_t( STDERR_FILENO, _size ); }

    file_t   cin( const ulong& _size=CHUNK_SIZE ){ return file_t( STDIN_FILENO, _size ); }
    
    /*─······································································─*/

    string_t read_file( const string_t& path ){ string_t s;
        if( path.empty() ) return s;
        file_t _file( path, "r" );
        while( _file.is_available() )
             { s +=_file.read(); } return s;
    }
    
    /*─······································································─*/

    int copy_file( const string_t& src, const string_t& des ){ 
        if( src.empty() || des.empty() ) return -1;
        try {
            file_t _file_a ( src, "r" );
            file_t _file_b ( des, "w" );
            stream::pipe( _file_a, _file_b ); 
                       return  1;
        } catch(...) { return -1; }
    }
    
    /*─······································································─*/

    int rename_file( const string_t& oname, const string_t& nname ) {
        if( oname.empty() || nname.empty() ) return -1;
        return rename( oname.c_str(), nname.c_str() );
    }
    
    /*─······································································─*/

    int move_file( const string_t& oname, const string_t& nname ) {
        return rename_file( oname, nname );
    }
    
    /*─······································································─*/

    int remove_file( const string_t& path ){
        if( path.empty() ) return -1;
        return remove( path.c_str() );
    }
    
    /*─······································································─*/

    bool exists_file( const string_t& path ){
        if( path.empty() ) return 0;
        try{file_t( path, "r" );
                      return 1;
        } catch(...){ return 0; }
    }
    
    /*─······································································─*/

    int create_file( const string_t& path ){
        if( path.empty() ) return -1;
        try{file_t( path, "w+" );
                      return 1;
        } catch(...){ return 0; }
    }
    
    /*─······································································─*/

    ulong file_size( const string_t& path ){
        try { file_t file( path, "r" );
              return file.size();
        } catch(...) { return 0; }
    }
    
    /*─······································································─*/

    void write_file( const string_t& path, const string_t& data ){
        file_t file( path, "w" ); file.write( data );
    }
    
    /*─······································································─*/

    void append_file( const string_t& path, const string_t& data ){
        file_t file( path, "a" ); file.write( data );
    }
    
    /*─······································································─*/

    int rename_folder( const string_t& oname, const string_t& nname ) { 
        return rename_file( oname, nname );
    }
    
    /*─······································································─*/

    int move_folder( const string_t& oname, const string_t& nname ){ 
        return rename_file( oname, nname );
    }
    
    /*─······································································─*/

    int create_folder( const string_t& path, uint permission=0777 ){ 
        if( path.empty() ){ return -1; }
        return mkdir( path.c_str(), permission );
    }
    
    /*─······································································─*/

    int remove_folder( const string_t& path ){ 
        if( path.empty() ){ return -1; }
        return rmdir( path.c_str() );
    }
    
    /*─······································································─*/

    bool exists_folder( const string_t& path ){
        if( path.empty() ){ return -1; }
        DIR* dir = opendir( path.c_str() );
        if( dir ) { return closedir(dir); }
                    return 0;
    }
    
    /*─······································································─*/

    array_t<string_t> read_folder( const string_t& path ){ 
        if( path.empty() ) $Error("Error path is empty"); 
        DIR* dir = opendir( path.c_str() ); struct dirent* entry;

        array_t<string_t> list; 

        if( dir ) {
            while ((entry = readdir(dir)) != NULL) {
		if( string_t(entry->d_name) == ".." ) continue;
		if( string_t(entry->d_name) == "."  ) continue;
                list.push( entry->d_name );
            }   closedir(dir);
        } else {
            $Error("Error opening directory"); 
        }   return list;            
    }
    
    /*─······································································─*/

    long folder_size( const string_t& path ){
          auto list = read_folder( path );
        return list.size(); 
    }

    /*─······································································─*/

    bool is_folder( const string_t& path ){ return exists_folder(path); }
    
    bool   is_file( const string_t& path ){ return exists_file(path); }

    /*─······································································─*/

    int copy_folder( const string_t& opath, const string_t& npath ){
        auto cmd = string::format( "cp -R %s %s", (char*)opath, (char*)npath );
        return ::system( cmd.c_str() );
    }
    
}}

/*────────────────────────────────────────────────────────────────────────────*/
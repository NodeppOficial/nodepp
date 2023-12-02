#ifndef NODEPP_FS
#define NODEPP_FS

/*────────────────────────────────────────────────────────────────────────────*/

#include <sys/stat.h>
#include "process.h"
#include <dirent.h>
#include "stream.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace fs {
    
    file_t* readable_new( const string_t& path, ulong _size=CHUNK_SIZE ){ return new file_t( path, "r", _size ); }
    file_t  readable( const string_t& path, ulong _size=CHUNK_SIZE ){ return file_t( path, "r", _size ); }
    
    /*─······································································─*/

    file_t* writable_new( const string_t& path, ulong _size=CHUNK_SIZE ){ return new file_t( path, "w", _size ); }
    file_t  writable( const string_t& path, ulong _size=CHUNK_SIZE ){ return file_t( path, "w", _size ); }
    
    /*─······································································─*/

    file_t* tmp_new( const ulong& _size=CHUNK_SIZE ){ return new file_t( _size ); }
    file_t  tmp( const ulong& _size=CHUNK_SIZE ){ return file_t( _size ); }
    
    /*─······································································─*/

    string_t read_file( const string_t&  path ){ string_t s;
        if( path.empty() ) return s;
        file_t _file( path, "r" );
        while( _file.is_available() )
             { s +=_file.read(); } return s;
    }
    
    /*─······································································─*/

    int copy_file( const string_t&  src, const string_t&  des ){ 
        if( src.empty() || des.empty() ) return -1;
        try {
            file_t _file_a( src, "r" );
            file_t _file_b( des, "w" );
            stream::pipe( _file_a, _file_b ); return 1;
        } catch(...) { return -1; }
    }
    
    /*─······································································─*/

    int rename_file( const string_t&  oname, const string_t&  nname ) {
        if( oname.empty() || nname.empty() ) return -1;
        return rename( oname.c_str(), nname.c_str() );
    }
    
    /*─······································································─*/

    int move_file( const string_t&  oname, const string_t&  nname ) {
        return rename_file( oname, nname );
    }
    
    /*─······································································─*/

    int remove_file( const string_t&  path ){
        if( path.empty() ) return -1;
        return remove( path.c_str() );
    }
    
    /*─······································································─*/

    bool exists_file( const string_t&  path ){
        if( path.empty() ) return 0;
        FILE* _stream = fopen( path.c_str(), "r" );
        if( _stream == nullptr ) return 0;
        return fclose( _stream ) ? 0 : 1;
    }
    
    /*─······································································─*/

    int create_file( const string_t&  path ){
        if( path.empty() ) return -1;
        FILE* _stream = fopen( path.c_str(), "w" );
        return fclose( _stream );
    }
    
    /*─······································································─*/

    int file_size( const string_t&  path ){
        try { file_t file( path, "r" );
              return file.size();
        } catch(...) { return -1; }
    }
    
    /*─······································································─*/

    void write_file( const string_t&  path, const string_t&  data ){
        file_t file( path, "w" ); file.write( data );
    }
    
    /*─······································································─*/

    void append_file( const string_t&  path, const string_t&  data ){
        file_t file( path, "a" ); file.write( data );
    }
    
    /*─······································································─*/

    int rename_folder( const string_t&  oname, const string_t&  nname ) { 
        return rename_file( oname, nname );
    }
    
    /*─······································································─*/

    int move_folder( const string_t&  oname, const string_t&  nname ){ 
        return rename_file( oname, nname );
    }
    
    /*─······································································─*/

    int create_folder( const string_t&  path, uint permission=0777 ){ 
        if( path.empty() ) return -1; 
        return mkdir( path.c_str(), permission );
    }
    
    /*─······································································─*/

    int remove_folder( const string_t&  path ){ 
        if( path.empty() ) return -1; 
        return rmdir( path.c_str() );
    }
    
    /*─······································································─*/

    bool exists_folder( const string_t&  path ){
        if( path.empty() ) return 0;
        DIR* dir = opendir( path.c_str() );
        if( dir ) { closedir(dir); return 1; }
                                   return 0;
    }
    
    /*─······································································─*/

    array_t<string_t> read_folder( const string_t&  path ){ 
        if( path.empty() ) _Error("Error path is empty"); 
        DIR* dir = opendir( path.c_str() ); struct dirent* entry;

        array_t<string_t> list; 

        if( dir ) {
            while ((entry = readdir(dir)) != NULL) {
		if( string_t(entry->d_name) == ".." ) continue;
		if( string_t(entry->d_name) == "."  ) continue;
                list.push( entry->d_name );
            }   closedir(dir);
        } else {
            _Error("Error opening directory"); 
        }   return list;            
    }
    
    /*─······································································─*/

    long folder_size( const string_t&  path ){
          auto list = read_folder( path );
        return list.size(); 
    }

    /*─······································································─*/

    bool is_folder( const string_t&  path ){ return exists_folder(path); }
    
    bool   is_file( const string_t&  path ){ return exists_file(path); }

    /*─······································································─*/

    int copy_folder( const string_t& opath, const string_t&  npath ){
        return ::system( string::format( "cp -R %s %s", (char*)opath, (char*)npath ).c_str() );
    }
    
}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
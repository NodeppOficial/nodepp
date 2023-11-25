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
    
    file_t* readable_new( string_t path, ulong _size=CHUNK_SIZE ){ return new file_t( path, "r", _size ); }
    file_t  readable( string_t path, ulong _size=CHUNK_SIZE ){ return file_t( path, "r", _size ); }
    
    /*─······································································─*/

    file_t* writable_new( string_t path, ulong _size=CHUNK_SIZE ){ return new file_t( path, "w", _size ); }
    file_t  writable( string_t path, ulong _size=CHUNK_SIZE ){ return file_t( path, "w", _size ); }
    
    /*─······································································─*/

    file_t* tmp_new( const ulong& _size=CHUNK_SIZE ){ return new file_t( _size ); }
    file_t  tmp( const ulong& _size=CHUNK_SIZE ){ return file_t( _size ); }
    
    /*─······································································─*/

    string_t read_file( string_t path ){ string_t s;
        if( path.empty() ) return s;
        file_t _file( path, "r" );
        while( _file.is_available() )
             { s +=_file.read(); } return s;
    }
    
    /*─······································································─*/

    int copy_file( string_t src, string_t des ){ 
        if( src.empty() || des.empty() ) return -1;
        try {
            file_t _file_a( src, "r" );
            file_t _file_b( des, "w" );
            stream::pipe( _file_a, _file_b ); return 1;
        } catch(...) { return -1; }
    }
    
    /*─······································································─*/

    int rename_file( string_t oname, string_t nname ) {
        if( oname.empty() || nname.empty() ) return -1;
        return rename( oname.c_str(), nname.c_str() );
    }
    
    /*─······································································─*/

    int move_file( string_t oname, string_t nname ) {
        return rename_file( oname, nname );
    }
    
    /*─······································································─*/

    int remove_file( string_t path ){
        if( path.empty() ) return -1;
        return remove( path.c_str() );
    }
    
    /*─······································································─*/

    bool exists_file( string_t path ){
        if( path.empty() ) return 0;
        FILE* _stream = fopen( path.c_str(), "r" );
        if( _stream == nullptr ) return 0;
        return fclose( _stream ) ? 0 : 1;
    }
    
    /*─······································································─*/

    int create_file( string_t path ){
        if( path.empty() ) return -1;
        FILE* _stream = fopen( path.c_str(), "w" );
        return fclose( _stream );
    }
    
    /*─······································································─*/

    int file_size( string_t path ){
        try { file_t file( path, "r" );
              return file.size();
        } catch(...) { return -1; }
    }
    
    /*─······································································─*/

    void write_file( string_t path, string_t data ){
        file_t file( path, "w" ); file.write( data );
    }
    
    /*─······································································─*/

    void append_file( string_t path, string_t data ){
        file_t file( path, "a" ); file.write( data );
    }
    
    /*─······································································─*/

    int rename_folder( string_t oname, string_t nname ) { 
        return rename_file( oname, nname );
    }
    
    /*─······································································─*/

    int move_folder( string_t oname, string_t nname ){ 
        return rename_file( oname, nname );
    }
    
    /*─······································································─*/

    int create_folder( string_t path, uint permission=0777 ){ 
        if( path.empty() ) return -1; 
        return mkdir( path.c_str(), permission );
    }
    
    /*─······································································─*/

    int remove_folder( string_t path ){ 
        if( path.empty() ) return -1; 
        return rmdir( path.c_str() );
    }
    
    /*─······································································─*/

    bool exists_folder( string_t path ){
        if( path.empty() ) return 0;
        DIR* dir = opendir( path.c_str() );
        if( dir ) { closedir(dir); return 1; }
                                   return 0;
    }
    
    /*─······································································─*/

    array_t<string_t> read_folder( string_t path ){ 
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

    long folder_size( string_t path ){
        if( path.empty() ){ return -1; }
        DIR* dir = opendir( path.c_str() ); struct dirent* entry;
        ulong count=0; if( dir ) {
            while ((entry = readdir(dir)) != NULL ){ count++; } closedir(dir);
        } else { return -1; } return count-2;            
    }

    /*─······································································─*/

    bool is_folder( string_t path ){
        DIR* dir = opendir(path.c_str());
        if( dir ) { closedir(dir); return 1;
        } else { return 0; }
    }

    bool is_file( string_t path ){
        DIR* dir = opendir(path.c_str());
        if( dir ) { closedir(dir); return 0;
        } else { return 1; }
    }

    /*─······································································─*/

    int readlink( string_t path, char* buff, ulong size ){ return ::readlink( path.c_str(), buff, size ); }
    int  symlink( string_t opath, string_t npath ){ return ::symlink( opath.c_str(), npath.c_str() ); }
    int     link( string_t opath, string_t npath ){ return ::link( opath.c_str(), npath.c_str() ); }
    int   unlink( string_t path ){ return ::unlink( path.c_str() ); }

    /*─······································································─*/

    int copy_folder( string_t opath, string_t npath ){
    #ifndef linux
        return ::system( string::format( "COPY %s %s", (char*)opath, (char*)npath ).c_str() );
    #else
        return ::system( string::format( "cp %s %s", (char*)opath, (char*)npath ).c_str() );
    #endif
    }
    
}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
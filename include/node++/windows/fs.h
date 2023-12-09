#pragma once
#define TMP_FILE string::format("%s%s",os::tmp().c_str(),"nodepp_XXXXXX").data()

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace fs {

    file_t  readable( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return file_t( path, "r", _size ); }

    file_t  writable( const string_t& path, const ulong& _size=CHUNK_SIZE ){ return file_t( path, "w", _size ); }

    file_t  cout( const ulong& _size=CHUNK_SIZE ){ return file_t( GetStdHandle(STD_OUTPUT_HANDLE), _size ); }
    
    file_t  cerr( const ulong& _size=CHUNK_SIZE ){ return file_t( GetStdHandle(STD_ERROR_HANDLE), _size ); }

    file_t   cin( const ulong& _size=CHUNK_SIZE ){ return file_t( GetStdHandle(STD_INPUT_HANDLE), _size ); }

    file_t   tmp( const ulong& _size=CHUNK_SIZE ){ return file_t( mktemp(TMP_FILE), "w+", _size ); }
    
    /*─······································································─*/

    string_t read_file( const string_t& path ){
        if( path.empty() ) return ""; string_t s;
        file_t _file( path, "r" );
        while( _file.is_available() )
             { s +=_file.read(); } return s;
    }
    
    /*─······································································─*/

    int copy_file( const string_t& src, const string_t& des ){ 
        return CopyFileA( src.c_str(), des.c_str(), 0 )!=0 ? -1 : 0;
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

    int create_folder( const string_t& path, uint permission /*unused*/ ){
        if( path.empty() ){ return -1; } 
        return CreateDirectoryA( path.c_str(), NULL )!=0 ? -1 : 0;
    }
    
    /*─······································································─*/

    int remove_folder( const string_t& path ){ 
        if( path.empty() ){ return -1; } 
        return RemoveDirectoryA( path.c_str() )!=0 ? -1 : 0;
    }
    
    /*─······································································─*/

    bool exists_folder( const string_t& path ){
        if( path.empty() ){ return -1; } 
        DWORD attributes = GetFileAttributesA( path.c_str() );
        return ( attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) );
    }
    
    /*─······································································─*/

    array_t<string_t> read_folder( const string_t& path ){ 
        if( path.empty() ) $Error("Error path is empty"); 

        LPWIN32_FIND_DATAA findData; array_t<string_t> list; 
        HANDLE hFind = FindFirstFileA( path.c_str(), findData );

        if( hFind == INVALID_HANDLE_VALUE )
            $Error("Error: Failed to open directory");

        do {
            string_t fileName = findData->cFileName;
            if( fileName != "." && fileName != ".." ){
                list.push( fileName );
            }
        } while ( FindNextFileA(hFind,findData) != 0 );

        FindClose( hFind ); return list;
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

    int copy_folder( const string_t& opath, const string_t&  npath ){
        return CopyFileA( opath.c_str(), npath.c_str(), 0 )!=0 ? -1 : 0;
    }
    
}}

/*────────────────────────────────────────────────────────────────────────────*/
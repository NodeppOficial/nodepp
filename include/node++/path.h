#ifndef NODEPP_PATH
#define NODEPP_PATH

/*────────────────────────────────────────────────────────────────────────────*/

#include "regex.h"
#include "map.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

struct path_t {
    string_t path;
    string_t root; 
    string_t base;
    string_t name;
    string_t dir;
    string_t ext;
};

/*────────────────────────────────────────────────────────────────────────────*/

namespace _path_ { map_t<string_t,string_t> mimetype ({

        { "txt", "text/plain" },
        { "text", "text/plain" },

        { "otf", "font/otf" },
        { "ttf", "font/ttf" },
        { "woff", "font/woff" },
        { "woff2", "font/woff2" },

        { "oga", "audio/ogg" },
        { "aac", "audio/aac" },
        { "wav", "audio/wav" },
        { "mp3", "audio/mpeg" },
        { "opus", "audio/opus" },
        { "weba", "audio/webm" },

        { "ogv", "video/ogg" },
        { "mp4", "video/mp4" },
        { "ts", "video/mp2t" },
        { "webm", "video/webm" },
        { "mpeg", "video/mpeg" },
        { "avi", "video/x-msvideo" },

        { "cpp", "text/X-C" },
        { "css", "text/css" },
        { "csv", "text/csv" },
        { "html", "text/html" },
        { "scss", "text/scss" },
        { "ics", "text/calendar" },
        { "js", "text/javascript" },
        { "xml", "application/xhtml+xml" },

        { "bmp", "image/bmp" },
        { "gif", "image/gif" },
        { "png", "image/png" },
        { "jpg", "image/jpeg" },
        { "jpeg", "image/jpeg" },
        { "webp", "image/webp" },
        { "svg", "image/svg+xml" },
        { "ico", "image/vnd.microsoft.icon" },

        { "zip", "application/zip" },
        { "gz", "application/gzip" },
        { ".h", "application/x-.h" },
        { "json", "application/json" },
        { "wasm", "application/wasm" },
        { "tar", "application/x-tar" },
        { "rar", "application/vnd.rar" },
        { "7z", "application/x-7z-compressed" },
        { "m3u8", "application/vnd.apple.mpegurl" },
        { "exe", "application/vnd.microsoft.portable-executable" },

        { "pdf", "application/pdf" },
        { "doc", "application/msword" },
        { "vsd", "application/vnd.visio" },
        { "xls", "application/vnd.ms-excel" },
        { "ppt", "application/vnd.ms-powerpoint" },
        { "swf", "application/x-shockwave-fla.h" },
        { "ods", "application/vnd.oasis.opendocument.spreadsheet" },
        { "odp", "application/vnd.oasis.opendocument.presentation" },
        { "odt", "application/vnd.oasis.opendocument.presentation" },
        { "xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" },
        { "docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document" },
        { "pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation" }

}); }

/*────────────────────────────────────────────────────────────────────────────*/

namespace path {

    #ifndef linux
        string_t sep  = "\\\\";
        string_t root = "c:\\\\";
        string_t none = "[\\\\]+";
        string_t one  = "[^\\\\]+";
        string_t init = "\\w:\\\\";
        string_t _beg = "\\w:\\\\";
        string_t beg  = "^\\w:\\\\";
    #else
        string_t init = "";
        string_t sep  = "/";
        string_t _beg = "/";
        string_t root = "./";
        string_t beg  = "^/";
        string_t none = "[/]+";
        string_t one  = "[^/]+";
    #endif
    
    /*─······································································─*/

    bool is_absolute( string_t path ){
        regex_t reg("^"+beg); return reg.test( path );
    }
    
    /*─······································································─*/

    string_t extname( string_t path ){ string_t m;
        regex_t reg("\\.\\w*$"); if( !reg.test( path ) ) 
          { return m; } m = reg.match( path ); 
            return regex::replace_all( m, "\\.", "" );
    }
    
    /*─······································································─*/

    string_t mimetype( string_t path ){
        string_t ext = extname( path ); if( ext.empty() ) 
        { return ext; } for( auto x: _path_::mimetype ){
            if( regex::test( ext, x.first ) ){ return x.second; }
        }   return regex::format("aplication/${0}",ext);
    }

    /*─······································································─*/

    string_t dirname( string_t path ){ 
        auto vec = regex::split( path, none );
        vec.pop(); return vec.join( sep );
    }
    
    /*─······································································─*/

    string_t basename( string_t path ){ 
        auto vec = regex::match_all( path, one );
        return vec[ vec.size()-1 ];
    }
    
    /*─······································································─*/

    string_t basename( string_t path, string_t del ){ 
        auto vec = regex::match_all( path, one );
        return regex::replace( vec[ vec.last() ], del, "" );
    }

    /*─······································································─*/

    string_t format( path_t* obj=nullptr ) { string_t _path;

        if( obj == nullptr ){ return _path; }
        if( !obj->path.empty() ){ return obj->path; }
        
        if( !obj->root.empty() ){ _path += obj->root; }
        else                    { _path += root; }

        if( !obj->dir.empty()  ){ _path += obj->dir; }
        if( !obj->base.empty() ){ _path += obj->base; }

        else {
            if( !obj->name.empty() ){ _path += obj->name + string::to_string("."); }
            if( !obj->ext.empty() ) { _path += obj->ext; }
        }
        
        return _path;
    }
    
    /*─······································································─*/

    path_t parse( string_t path ) { path_t result;

        if( regex::test( path, beg ) ) result.root = _beg;
        else                           result.root = root;

        result.path = path;
        result.ext  = extname( path ); 
        result.dir  = dirname( path );
        result.base = basename( path );
        result.name = basename( path, "\\."+result.ext );

        return result;
    };

    /*─······································································─*/

    string_t relative( string_t path_a, string_t path_b ){
        regex_t _a(sep+"+"), _b(init,"i");
        auto vec_a = _a.split( path_a );
        for( auto x:vec_a ){
            if( x.empty() )                continue;
            if( _b.test( x ) )             continue;
            if(!regex::test( path_b, x ) ) break;
            path_b = regex::replace( path_b, x, ".." );
        }   return path_b;
    }  
    
    /*─······································································─*/

    string_t normalize( string_t path="" ){ 

        regex_t _d("[.]{3}(/|\\\\\\\\)[^./\\\\]+(/|\\\\\\\\)");
        regex_t _a("[.]{3}(/|\\\\\\\\)[^./\\\\]+");
        regex_t _c("[.]{3}(/|\\\\\\\\)");
        regex_t _b( sep + "+" );
        
        string_t null("."); if( path.empty() ) return null;
        path = _b.replace_all( path, sep );
        
        while( _a.test( path ) ){ path = _d.replace_all( path, "" ); }
        while( _c.test( path ) ){ path = _c.replace_all( path, "" ); }
        return path;
    }
    
    /*─······································································─*/

    template< class T, class... V > string_t join( const T& argc, const V&... args ){ 
        return normalize( string::join( sep, argc, args... ) ); 
    }
    
}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
#if !defined(GENERATOR_TIMER) && defined(NODEPP_TIMER)
    #define  GENERATOR_TIMER
namespace nodepp { namespace _timer_ {

    _Generator( timer ){ public:

        template< class V, class... T > 
        _Emit( V func, const ptr_t<ulong>& out, ulong time, const T&... args ){
        _GStart
            if(*out == 0 )                   _End;
            if( process::millis() <= *out )  _Goto(0);
            if( func(args...)<0 )            _End;
            *out = process::millis() + time; _Goto(0); 
        _GStop
        }

        template< class V, class... T > 
        _Emit( V func, const ptr_t<ulong>& out, ulong* time, const T&... args ){
        _GStart
            if(*out == 0 )                   _End;
            if( process::millis() <= *out )  _Goto(0);
            if( func(args...)<0 )            _End;
            *out = process::millis() +*time; _Goto(0); 
        _GStop
        }

    };
    
    /*─······································································─*/

    _Generator( utimer ){ public:

        template< class V, class... T > 
        _Emit( V func, const ptr_t<ulong>& out, ulong time, const T&... args ){
        _GStart
            if(*out == 0 )                   _End;
            if( process::micros() <= *out )  _Goto(0);
            if( func(args...)<0 )            _End;
            *out = process::micros() + time; _Goto(0);
        _GStop
        }

        template< class V, class... T > 
        _Emit( V func, const ptr_t<ulong>& out, ulong* time, const T&... args ){
        _GStart
            if(*out == 0 )                   _End;
            if( process::micros() <= *out )  _Goto(0);
            if( func(args...)<0 )            _End;
            *out = process::micros() +*time; _Goto(0);
        _GStop
        }

    };

}}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_FILE) && defined(NODEPP_FILE)
    #define  GENERATOR_FILE
namespace nodepp { namespace _file_ {

    _Generator( read ){ public: 

        ulong*   r;
        string_t y;
        int      c; 
        ulong    d;

    template< class T > _Emit( T* str, ulong size=CHUNK_SIZE ){
    _GStart c=0; d=0; y.clear(); str->flush();

        if( !str->is_available() ){ str->close(); _End; } r = str->get_range();
        if( !str->get_borrow().empty() ){ y=str->get_borrow(); str->del_borrow(); }

          if ( r[1] != 0 ){ auto pos = str->pos(); d = r[1]-r[0];
          if ( pos < r[0] ){ str->del_borrow(); str->pos( r[0] ); }
        elif ( pos >=r[1] ){ str->close(); _End; } }
        else { d = str->get_buffer_size(); }

        if( y.empty() ) do {
                 c =str->_read( str->get_buffer_data(),min(d,size) );
             if( true /* c==-2 */ ){ _Next; }
        } while( c==-2 );
        
        if( c<=0 && y.empty() ){ str->close(); _End; } elif ( c>0 ){
            y = (string_t){ str->get_buffer_data(), (ulong) c };
        }   c = y.size();
        
    _GStop
    }};
    
    /*─······································································─*/

    _Generator( write ){ public:

        ulong    y = 0; 
        int      c = 0;
        ulong size = 0;
        
    template< class T > _Emit( T* str, const string_t& msg ){
    _GStart c=0; y=0; str->flush(); str->del_borrow();

        if( !str->is_available() || msg.empty() ){ str->close(); _End; } 
        if(  str->get_borrow().empty() ){ str->set_borrow( msg ); }
        
        do { do { c = str->_write(str->get_borrow_data()+y,str->get_borrow_size()-y);
             if ( true /* c==-2 */ ){ _Next; }
        } while ( c==-2 ); if( c>0 ){ y += c; }
        } while ( c>=0 && y<str->get_borrow_size() ); str->del_borrow();
        
        if( c<=0 ){ str->close(); _End; }
        
    _GStop
    }};

    /*─······································································─*/

    _Generator( line ){ public: 
    
        _file_::read prs;
        string_t     s,y;  
        ulong          c; 

    template< class T > _Emit( T* str ){
    _GStart c=1; s.clear(); y.clear(); str->flush();

        while( str->is_available() ){
        while( prs(str) == 1 ){ _Next; }
           if( prs.c<=0 ){ break; } c=1; s += prs.y; 
          for( auto x:s ){ if( x == '\n' ){ break; } c++; }
           if( c<=s.size() ){ break; }
        }      str->set_borrow(s);

        y = str->get_borrow().splice( 0, c );
    
    _GStop
    }};

}}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_STREAM) && defined(NODEPP_STREAM)
    #define  GENERATOR_STREAM 
namespace nodepp { namespace _stream_ {

    _Generator( pipe ){ public:

        _file_::write _write;
        _file_::read  _read;

        template< class T > _Emit( const T& inp ){
        _GStart inp.onPipe.emit();
            while( inp.is_available() ){
            while( _read(&inp)==1 ){ _Next; } 
               if( _read.c <= 0 )  { break; }
                    inp.onData.emit( _read.y );
            }       inp.close();
        _GStop
        }

        template< class T, class V > _Emit( const T& inp, const V& out ){
        _GStart inp.onPipe.emit();
            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 )         { _Next; }
               if( _read.c <= 0 )           { break; }
            while( _write(&out,_read.y)==1 ){ _Next; }
                    inp.onData.emit( _read.y );
            }       inp.close(); out.close();
        _GStop
        }

    };
    
    /*─······································································─*/

    _Generator( line ){ public:

        _file_::write _write;
        _file_::line  _read;

        template< class T > _Emit( const T& inp ){
        _GStart inp.onPipe.emit();
            while( inp.is_available() ){
            while( _read(&inp)==1 ){ _Next; } 
               if( _read.c <= 0 )  { break; }
                   inp.onData.emit( _read.y );
            }      inp.close();
        _GStop
        }

        template< class T, class V > _Emit( const T& inp, const V& out ){
        _GStart inp.onPipe.emit();
            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 )         { _Next; } 
               if( _read.c <= 0 )           { break; }
            while( _write(&out,_read.y)==1 ){ _Next; }
                    inp.onData.emit( _read.y );
            }       inp.close(); out.close();
        _GStop
        }

    };
    
}}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_ZLIB) && defined(NODEPP_ZLIB)
    #define  GENERATOR_ZLIB 
namespace nodepp { namespace _zlib_ {

    _Generator( inflate ){ public:
    
        ptr_t<z_stream> str = new z_stream;
        int x=0; ulong size; string_t dout;
        _file_::write _write;
        _file_::read  _read;
        

        template< class T, class V, class U >
        _Emit( const T& inp, const V& out, U cb ){
        _GStart inp.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                _EError( inp.onError, message );
                _EError( inp.onError, message ); _End;
            }

            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 ){ _Next; }
               if( _read.c <= 0 )  { break; }

                str->avail_in = _read.y.size();
                str->avail_out= inp.get_buffer_size();
                str->next_in  = (Bytef*)_read.y.data();
                str->next_out = (Bytef*)inp.get_buffer_data(); 
                            x = ::inflate( &str, Z_FINISH );

                if(( size=inp.get_buffer_size()-str->avail_out )>0){
                    dout = (string_t){ inp.get_buffer_data(), size };
                    inp.onData.emit(dout); 
                    while( _write(&out,dout)==1 ){ _Next; } continue;
                }
                
                if( x==Z_STREAM_END ) { break; } elif( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    _EError( inp.onError, message );
                    _EError( out.onError, message ); break;
                }
            
            }   inflateEnd( &str ); out.close(); inp.close(); 
        
        _GStop
        }

        template< class T, class U >
        _Emit( const T& inp, U cb ){
        _GStart inp.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                _EError( inp.onError, message ); _End;
            }

            while( inp.is_available() ){
            while( _read(&inp)==1 ){ _Next; }
               if( _read.c <= 0 )  { break; }

                str->avail_in = _read.y.size();
                str->avail_out= inp.get_buffer_size();
                str->next_in  = (Bytef*)_read.y.data();
                str->next_out = (Bytef*)inp.get_buffer_data(); 
                            x = ::inflate( &str, Z_PARTIAL_FLUSH );

                if(( size=inp.get_buffer_size()-str->avail_out )>0){
                    dout = (string_t){ inp.get_buffer_data(), size };
                    inp.onData.emit(dout); continue;
                }

                if( x==Z_STREAM_END ) { break; } elif( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    _EError( inp.onError, message ); inp.close(); break;
                } 

            }   inflateEnd( &str ); inp.close(); 
            
        _GStop
        }

    };
    
    /*─······································································─*/

    _Generator( deflate ){ public:
    
        ptr_t<z_stream> str = new z_stream;
        int x=0; ulong size; string_t dout;
        _file_::write _write;
        _file_::read  _read;
        

        template< class T, class V, class U >
        _Emit( const T& inp, const V& out, U cb ){
        _GStart inp.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                _EError( inp.onError, message );
                _EError( inp.onError, message ); _End;
            }

            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 ){ _Next; }
               if( _read.c <= 0 )  { break; }

                str->avail_in = _read.y.size();
                str->avail_out= inp.get_buffer_size();
                str->next_in  = (Bytef*)_read.y.data();
                str->next_out = (Bytef*)inp.get_buffer_data(); 
                            x = ::deflate( &str, Z_PARTIAL_FLUSH );

                if(( size=inp.get_buffer_size()-str->avail_out )>0){
                    dout = (string_t){ inp.get_buffer_data(), size };
                    inp.onData.emit(dout); 
                    while( _write(&out,dout)==1 ){ _Next; } continue;
                }

                if( x==Z_STREAM_END ) { break; } elif( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    _EError( inp.onError, message );
                    _EError( out.onError, message ); break;
                }
            
            }   deflateEnd( &str ); out.close(); inp.close(); 
            
        _GStop
        }

        template< class T, class U >
        _Emit( const T& inp, U cb ){
        _GStart inp.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                _EError( inp.onError, message ); _End;
            }

            while( inp.is_available() ){
            while( _read(&inp)==1 ){ _Next; }
               if( _read.c <= 0 )  { break; }

                str->avail_in = _read.y.size();
                str->avail_out= inp.get_buffer_size();
                str->next_in  = (Bytef*)_read.y.data();
                str->next_out = (Bytef*)inp.get_buffer_data(); 
                            x = ::deflate( &str, Z_PARTIAL_FLUSH );

                if(( size=inp.get_buffer_size()-str->avail_out )>0){
                    dout = (string_t){ inp.get_buffer_data(), size };
                    inp.onData.emit(dout); continue;
                }
                
                if( x==Z_STREAM_END ) { break; } elif( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    _EError( inp.onError, message ); inp.close(); break;
                } 

            }   deflateEnd( &str ); inp.close(); 
            
        _GStop
        }

    };

}}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_WS) && defined(NODEPP_WS)
    #define  GENERATOR_WS
namespace nodepp { 

    bool WSServer( http_t cli ) {
        auto data = cli.read(); cli.set_borrow( data ); int c=0;
        
        while(( c=cli.read_header() )>0 ){ process::next(); }
           if(  c == -1  ){ return 0; }

        if( !cli.headers["Sec-Websocket-Key"].empty() ){

            string_t sec = regex::match(cli.headers["Sec-Websocket-Key"],"[^\\s\n ]+");
            string_t key = sec + SECRET;

                auto sha = crypto::hash::SHA1();  sha.update(key);
                auto b64 = crypto::enc::BASE64(); b64.update(sha.get());
                auto enc = b64.get().slice(0,-1);

            cli.write_headers( 101, {{
                { "Sec-Websocket-Accept", enc },
                { "Connection", "Upgrade" },
                { "Upgrade", "Websocket" }
            }});

            cli.stop();             return 1;
        }   cli.set_borrow( data ); return 0;

    }
    
    /*─······································································─*/

    template< class T > socket_t WSClient( const T& fetch, const string_t& key ) {
        auto res = fetch.await(); if( tuple::get<0>(res) == 1 ) _Error( tuple::get<2>(res).what() );
        if( tuple::get<0>(res) == 1 ){ _Error("Cant start ws client"); }
        auto cli = tuple::get<1>(res);

        if( cli.status != 101 ){ 
            _EError(cli.onError,string::format("Can't connect to WS Server -> status %d",cli.status)); 
            cli.close(); return cli; 
        }

        if(!cli.headers["Sec-Websocket-Accept"].empty() ){

            string_t dta = regex::match(cli.headers["Sec-Websocket-Accept"],"[^\\s\n ]+");
            string_t sec = key + SECRET;

                auto sha = crypto::hash::SHA1();  sha.update(sec);
                auto b64 = crypto::enc::BASE64(); b64.update(sha.get());
                auto enc = b64.get().slice(0,-1);

        if( dta != enc ){ _Error("secret key does not match"); }
            cli.stop();
        }   return cli;

    }

}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_WSS) && defined(NODEPP_WSS)
    #define  GENERATOR_WSS
namespace nodepp {
    
    bool WSServer( https_t cli ) {
        auto data = cli.read(); cli.set_borrow( data ); int c=0;
        
        while(( c=cli.read_header() )>0 ){ process::next(); }
           if(  c == -1  ){ return 0; }

        if( !cli.headers["Sec-Websocket-Key"].empty() ){

            string_t sec = regex::match(cli.headers["Sec-Websocket-Key"],"[^\\s\n ]+");
            string_t key = sec + SECRET;

                auto sha = crypto::hash::SHA1();  sha.update(key);
                auto b64 = crypto::enc::BASE64(); b64.update(sha.get());
                auto enc = b64.get().slice(0,-1);

            cli.write_headers( 101, {{
                { "Sec-Websocket-Accept", enc },
                { "Connection", "Upgrade" },
                { "Upgrade", "Websocket" }
            }});

            cli.stop();             return 1;
        }   cli.set_borrow( data ); return 0;

    }
    
    /*─······································································─*/

    template< class T > ssocket_t WSClient( const T& fetch, const string_t& key ) {
        
        auto res = fetch.await(); if( tuple::get<0>(res) == 1 ) _Error( tuple::get<2>(res).what() );
        if( tuple::get<0>(res) == 1 ){ _Error("Cant start ws client"); }
        auto cli = tuple::get<1>(res);

        if( cli.status != 101 ){ 
            _EError(cli.onError,string::format("Can't connect to WS Server -> status %d",cli.status)); 
            cli.close(); return cli; 
        }

        if(!cli.headers["Sec-Websocket-Accept"].empty() ){

            string_t dta = regex::match(cli.headers["Sec-Websocket-Accept"],"[^\\s\n ]+");
            string_t sec = key + SECRET;

                auto sha = crypto::hash::SHA1();  sha.update(sec);
                auto b64 = crypto::enc::BASE64(); b64.update(sha.get());
                auto enc = b64.get().slice(0,-1);

        if( dta != enc ){ _Error("secret key does not match"); }   
            cli.stop();
        }   return cli;

    }

}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_SWS) && ( defined(NODEPP_WS) || defined(NODEPP_WSS) )
    #define  GENERATOR_SWS
namespace nodepp {

    struct ws_frame_t {
        bool  FIN = 1; //1b
        uint  RSV = 0; //3b
        uint  OPC = 1; //4b
        bool  MSK = 1; //1b
        char  KEY [4]; //4B
        ulong LEN = 0; //64b
    };

    ulong write_ws_frame( char* bf, const ulong& sx ){
        static ulong len;

        if( bf    == nullptr    ){ return   0; }
        if( bf[0] == (char)0x81 ){ return len; }

        string_t y = string_t( bf, sx ); uint idx = 0; 

        bf[idx] = 0b10000001; idx++;
        bf[idx] = 0b00000000; // 0b10000000 MASKED

        if ( y.size() < 126 ){ 
            bf[idx]|= (uchar) y.size(); idx++;
        } elif ( y.size() < (uint) -1 ){ 
            bf[idx]|= (uchar) 126; idx++;
            bf[idx] = (uchar)(y.size() << ( 32 - 8  )); idx++;
            bf[idx] = (uchar)(y.size() << ( 32 - 16 )); idx++;
        } elif ( y.size() < (ulong) -1 ){
            bf[idx]|= (uchar) 127; idx++;
            bf[idx] = (uchar)(y.size() << ( 32 - 8  )); idx++;
            bf[idx] = (uchar)(y.size() << ( 32 - 16 )); idx++;
            bf[idx] = (uchar)(y.size() << ( 32 - 24 )); idx++;
            bf[idx] = (uchar)(y.size() << ( 32 - 32 )); idx++;
        }

        for( ulong x=0; x<y.size(); x++ ){
             bf[idx] = y[x]; idx++;
        }    
        
        len = idx; return idx; 
    }

    ulong read_ws_frame( char* bf, const ulong& sx ){

        if( bf == nullptr ){ return  0; }

        uint   idx = 0; ws_frame_t st;
        string_t y = string::to_bin( bf[idx] ); idx++;

        st.FIN = y.splice(0,1) == "1";

        for( auto x : y.splice(0,3) ) st.RSV = st.RSV<<1 | (x=='1');
        for( auto x : y.splice(0,4) ) st.OPC = st.OPC<<1 | (x=='1');

        y = string::to_bin( bf[idx] ); idx++; 
        st.MSK = y.splice(0,1) == "1"; 

        for( auto x : y.splice(0,7) ) st.LEN = st.LEN<<1 | (x=='1');
        if ( st.LEN == 126 ){ st.LEN = 0;
            st.LEN = st.LEN << 8 | (uchar) bf[idx]; idx++;
            st.LEN = st.LEN << 8 | (uchar) bf[idx]; idx++;
        } elif ( st.LEN == 127 ) { st.LEN = 0;
            st.LEN = st.LEN << 8 | (uchar) bf[idx]; idx++;
            st.LEN = st.LEN << 8 | (uchar) bf[idx]; idx++;
            st.LEN = st.LEN << 8 | (uchar) bf[idx]; idx++;
            st.LEN = st.LEN << 8 | (uchar) bf[idx]; idx++;
        }

        if ( st.MSK ) for( ulong x=0; x<4; x++ )
           { st.KEY[x] = bf[idx]; idx++; }

        if ( st.MSK ) for ( ulong x=0; x<st.LEN; x++ )
           { bf[x] = bf[idx] ^ st.KEY[x%4]; idx++; }
        else for ( ulong x=0; x<st.LEN; x++ )
           { bf[x] = bf[idx]; idx++; }

        if ( st.OPC == 24 ){ return 0; } 
        if ( st.OPC ==  8 ){ return 0; }

        return st.LEN; 
    }

}
#endif
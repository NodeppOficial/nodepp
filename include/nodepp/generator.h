/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#define NODEPP_GENERATOR

#if !defined(GENERATOR_TIMER) && defined(NODEPP_TIMER) && defined(NODEPP_GENERATOR)
    #define  GENERATOR_TIMER
namespace nodepp { namespace _timer_ {

    GENERATOR( timer ){ public:

        template< class V, class... T > 
        gnEmit( V func, ulong time, const T&... args ){
        gnStart
            coDelay( time ); if( func(args...)<0 )
            { coEnd; } coGoto(0); 
        gnStop
        }

        template< class V, class... T > 
        gnEmit( V func, ulong* time, const T&... args ){
        gnStart
            coDelay( *time ); if( func(args...)<0 )
            { coEnd; } coGoto(0); 
        gnStop
        }

    };
    
    /*─······································································─*/

    GENERATOR( utimer ){ public:

        template< class V, class... T > 
        gnEmit( V func, ulong time, const T&... args ){
        gnStart
            coUDelay( time ); if( func(args...)<0 )
            { coEnd; } coGoto(0);
        gnStop
        }

        template< class V, class... T > 
        gnEmit( V func, ulong* time, const T&... args ){
        gnStart
            coUDelay( *time ); if( func(args...)<0 )
            { coEnd; } coGoto(0);
        gnStop
        }

    };

}}  
#undef NODEPP_GENERATOR
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_FILE) && defined(NODEPP_FILE) && defined(NODEPP_GENERATOR)
    #define  GENERATOR_FILE
namespace nodepp { namespace _file_ {

    GENERATOR( read ){ 
    private:  
        ulong    d; 
        ulong*   r; 
        
    public: 
        string_t data ;
        int      state; 

    template< class T > gnEmit( T* str, ulong size=CHUNK_SIZE ){
    gnStart state=0; d=0; data.clear(); str->flush();

        if(!str->is_available() ){ coEnd; } r = str->get_range();
        if(!str->get_borrow().empty() ){ data = str->get_borrow(); }

          if ( r[1] != 0  ){ auto pos = str->pos(); d = r[1]-r[0];
          if ( pos < r[0] ){ str->del_borrow(); str->pos( r[0] ); }
        elif ( pos >=r[1] ){ coEnd; } }
        else { d = str->get_buffer_size(); }

        if( data.empty() ) do {
            state=str->_read( str->get_buffer_data(), min(d,size) );
        if( true /* state==-2 */ ){ coNext; } } while ( state==-2 );
        
        if( state > 0 ){
            data = string_t( str->get_buffer_data(), (ulong) state );
        }   state = data.size(); str->del_borrow();
        
    gnStop
    }};
    
    /*─······································································─*/

    GENERATOR( write ){ 
    private:
        string_t b ;

    public:
        ulong    data ; 
        int      state;
        
    template< class T > gnEmit( T* str, const string_t& msg ){
    gnStart state=0; data=0; str->flush();

        if(!str->is_available() || msg.empty() ){ coEnd; }
        if( b.empty() ){ b = msg; }
        
        do { do { state=str->_write( b.data()+data, b.size()-data );
             if ( true /* state==-2 */ )    { coNext;        }
        } while ( state==-2 ); if( state>0 ){ data += state; }
        } while ( state>=0 && data<b.size() ); b.clear();

    gnStop
    }};

    /*─······································································─*/

    GENERATOR( line ){ 
    private:
        _file_::read _read;
        string_t     s;

    public: 
        string_t  data ;  
        ulong     state; 

    template< class T > gnEmit( T* str ){
    gnStart state=1; s.clear(); data.clear(); str->flush();

        while( str->is_available() ){
        while( _read(str) == 1 ){ coNext; }
           if( _read.state<= 0 ){ break; } state = 1; s += _read.data; 
          for( auto &x: s )     { if( x == '\n' ){ break; } state++; }
           if( state<=s.size() ){ break; }
        }      str->set_borrow(s);

        data = str->get_borrow().splice( 0, state );
    
    gnStop
    }};

}}
#undef NODEPP_GENERATOR
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_STREAM) && defined(NODEPP_STREAM) && defined(NODEPP_GENERATOR)
    #define  GENERATOR_STREAM 
namespace nodepp { namespace _stream_ {

    GENERATOR( pipe ){ 
    private:

        _file_::write _write;
        _file_::read  _read;

    public:

        template< class T > gnEmit( const T& inp ){
        gnStart inp.onPipe.emit();
            while( inp.is_available() ){
            while( _read(&inp)==1 ){ coNext; }
               if( _read.state<=0 ){ break;  }
                    inp.onData.emit( _read.data );
            }       inp.close(); 
        gnStop
        }

        template< class T, class V > gnEmit( const T& inp, const V& out ){
        gnStart inp.onPipe.emit(); out.onPipe.emit();
            while( inp.is_available() && out.is_available() ){
            while( _read(&inp) ==1 )           { coNext; }
               if( _read.state <=0 )           { break;  }
            while( _write(&out,_read.data)==1 ){ coNext; }
               if( _write.state<=0 )           { break;  }
                    inp.onData.emit( _read.data );
            }       inp.close(); out.close();
        gnStop
        }

    };
    
    /*─······································································─*/

    GENERATOR( line ){ 
    private:

        _file_::write _write;
        _file_::line  _read;

    public:

        template< class T > gnEmit( const T& inp ){
        gnStart inp.onPipe.emit();
            while( inp.is_available() ){
            while( _read(&inp)==1 ){ coNext; } 
               if( _read.state<=0 ){ break;  }
                   inp.onData.emit( _read.data );
            }      inp.close(); 
        gnStop
        }

        template< class T, class V > gnEmit( const T& inp, const V& out ){
        gnStart inp.onPipe.emit(); out.onPipe.emit();
            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 )            { coNext; } 
               if( _read.state<=0 )            { break;  }
            while( _write(&out,_read.data)==1 ){ coNext; }
               if( _write.state<=0 )           { break;  }
                    inp.onData.emit( _read.data );
            }       inp.close(); out.close();
        gnStop
        }

    };
    
}}
#undef NODEPP_GENERATOR
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_ZLIB) && defined(NODEPP_ZLIB) && defined(NODEPP_GENERATOR)
    #define  GENERATOR_ZLIB 
namespace nodepp { namespace _zlib_ {

    GENERATOR( inflate ){ 
    private:
    
        ptr_t<z_stream> str = new z_stream;
        int x=0; ulong size; string_t dout;
        _file_::write _write;
        _file_::read  _read;

    public:

        template< class T, class V, class U >
        gnEmit( const T& inp, const V& out, U cb ){
        gnStart inp.onPipe.emit(); out.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                _EERROR( inp.onError, message );
                _EERROR( inp.onError, message ); coEnd;
            }

            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 ){ coNext; }
               if( _read.state<=0 ){ break;  }

                str->avail_in = _read.data.size();
                str->avail_out= inp.get_buffer_size();
                str->next_in  = (Bytef*)_read.data.data();
                str->next_out = (Bytef*)inp.get_buffer_data(); 
                            x = ::inflate( &str, Z_FINISH );

                if(( size=inp.get_buffer_size()-str->avail_out )>0){
                    dout = string_t( inp.get_buffer_data(), size );
                    inp.onData.emit(dout); 
                    while( _write(&out,dout)==1 ){ coNext; }
                       if( _write.state<=0 )     { break;  } continue;
                }
                
                if( x==Z_STREAM_END ) { break; } elif( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    _EERROR( inp.onError, message );
                    _EERROR( out.onError, message ); break;
                }
            
            }   inflateEnd( &str ); out.close(); inp.close(); 
        
        gnStop
        }

        template< class T, class U >
        gnEmit( const T& inp, U cb ){
        gnStart inp.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                _EERROR( inp.onError, message ); coEnd;
            }

            while( inp.is_available() ){
            while( _read(&inp)==1 ){ coNext; }
               if( _read.state<=0 ){ break;  }

                str->avail_in = _read.data.size();
                str->avail_out= inp.get_buffer_size();
                str->next_in  = (Bytef*)_read.data.data();
                str->next_out = (Bytef*)inp.get_buffer_data(); 
                            x = ::inflate( &str, Z_PARTIAL_FLUSH );

                if(( size=inp.get_buffer_size()-str->avail_out )>0){
                    dout = string_t( inp.get_buffer_data(), size );
                    inp.onData.emit(dout); continue;
                }

                if( x==Z_STREAM_END ) { break; } elif( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    _EERROR( inp.onError, message ); break;
                } 

            }   inflateEnd( &str ); inp.close(); 
            
        gnStop
        }

    };
    
    /*─······································································─*/

    GENERATOR( deflate ){ 
    private:

        ptr_t<z_stream> str = new z_stream;
        int x=0; ulong size; string_t dout;
        _file_::write _write;
        _file_::read  _read;

    public:

        template< class T, class V, class U >
        gnEmit( const T& inp, const V& out, U cb ){
        gnStart inp.onPipe.emit(); out.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                _EERROR( inp.onError, message );
                _EERROR( inp.onError, message ); coEnd;
            }

            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 ){ coNext; }
               if( _read.state<=0 ){ break;  }

                str->avail_in = _read.data.size();
                str->avail_out= inp.get_buffer_size();
                str->next_in  = (Bytef*)_read.data.data();
                str->next_out = (Bytef*)inp.get_buffer_data(); 
                            x = ::deflate( &str, Z_PARTIAL_FLUSH );

                if(( size=inp.get_buffer_size()-str->avail_out )>0){
                    dout = string_t( inp.get_buffer_data(), size );
                    inp.onData.emit(dout); 
                    while( _write(&out,dout)==1 ){ coNext; }
                       if( _write.state<=0 )     { break;  } continue;
                }

                if( x==Z_STREAM_END ) { break; } elif( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    _EERROR( inp.onError, message );
                    _EERROR( out.onError, message ); break;
                }
            
            }   deflateEnd( &str ); out.close(); inp.close(); 
            
        gnStop
        }

        template< class T, class U >
        gnEmit( const T& inp, U cb ){
        gnStart inp.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                _EERROR( inp.onError, message ); coEnd;
            }

            while( inp.is_available() ){
            while( _read(&inp)==1 ){ coNext; }
               if( _read.state<=0 ){ break;  }

                str->avail_in = _read.data.size();
                str->avail_out= inp.get_buffer_size();
                str->next_in  = (Bytef*)_read.data.data();
                str->next_out = (Bytef*)inp.get_buffer_data(); 
                            x = ::deflate( &str, Z_PARTIAL_FLUSH );

                if(( size=inp.get_buffer_size()-str->avail_out )>0){
                    dout = string_t( inp.get_buffer_data(), size );
                    inp.onData.emit(dout); continue;
                }
                
                if( x==Z_STREAM_END ) { break; } elif( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    _EERROR( inp.onError, message ); break;
                } 

            }   deflateEnd( &str ); inp.close(); 
            
        gnStop
        }

    };

}}
#undef NODEPP_GENERATOR
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_WS) && defined(NODEPP_WS) && defined(NODEPP_GENERATOR)
    #define  GENERATOR_WS
    #include "crypto.h"
namespace nodepp { 

    bool WSServer( http_t cli ) {
        auto data = cli.read(); cli.set_borrow( data ); int c=0;
        
        while(( c=cli.read_header() )>0 ){ process::next(); }
           if(  c == -1  ){ return 0; }

        if( !cli.headers["Sec-Websocket-Key"].empty() ){

            string_t sec = cli.headers["Sec-Websocket-Key"];
            string_t key = sec + SECRET;

                auto sha = crypto::hash::SHA1();
                auto b64 = crypto::encode::BASE64(); 

                     sha.update(key);
                     b64.update( crypto::hex2buff(sha.get()) );

                auto enc = b64.get().slice(0,-1);

            cli.write_header( 101, {{
                { "Sec-Websocket-Accept", enc },
                { "Connection", "Upgrade" },
                { "Upgrade", "Websocket" }
            }});

            cli.stop();             return 1;
        }   cli.set_borrow( data ); return 0;

    }
    
    /*─······································································─*/

    template< class T > socket_t WSClient( const T& fetch, const string_t& key ) {
        auto res = fetch.await(); if( !res.has_value() ) _ERROR( res.error().what() );
        auto cli = res.value();

        if( cli.status != 101 ){ 
            _EERROR(cli.onError,string::format("Can't connect to WS Server -> status %d",cli.status)); 
            cli.close(); return cli; 
        }

        if(!cli.headers["Sec-Websocket-Accept"].empty() ){

            string_t dta = cli.headers["Sec-Websocket-Accept"];
            string_t sec = key + SECRET;

                auto sha = crypto::hash::SHA1();
                auto b64 = crypto::encode::BASE64(); 

                     sha.update(key);
                     b64.update( crypto::hex2buff(sha.get()) );
                     
                auto enc = b64.get().slice(0,-1);

        if( dta != enc ){ _ERROR("secret key does not match"); }
            cli.stop();
        }   return cli;

    }

}
#undef NODEPP_GENERATOR
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_WSS) && defined(NODEPP_WSS) && defined(NODEPP_GENERATOR)
    #define  GENERATOR_WSS
    #include "crypto.h"
namespace nodepp {
    
    bool WSSServer( https_t cli ) {
        auto data = cli.read(); cli.set_borrow( data ); int c=0;
        
        while(( c=cli.read_header() )>0 ){ process::next(); }
           if(  c == -1  ){ return 0; }

        if( !cli.headers["Sec-Websocket-Key"].empty() ){

            string_t sec = cli.headers["Sec-Websocket-Key"];
            string_t key = sec + SECRET;

                auto sha = crypto::hash::SHA1();
                auto b64 = crypto::encode::BASE64(); 

                     sha.update(key);
                     b64.update( crypto::hex2buff(sha.get()) );
                     
                auto enc = b64.get().slice(0,-1);

            cli.write_header( 101, {{
                { "Sec-Websocket-Accept", enc },
                { "Connection", "Upgrade" },
                { "Upgrade", "Websocket" }
            }});

            cli.stop();             return 1;
        }   cli.set_borrow( data ); return 0;

    }
    
    /*─······································································─*/

    template< class T > ssocket_t WSSClient( const T& fetch, const string_t& key ) {
        auto res = fetch.await(); if( !res.has_value() ) _ERROR( res.error().what() );
        auto cli = res.value();

        if( cli.status != 101 ){ 
            _EERROR(cli.onError,string::format("Can't connect to WS Server -> status %d",cli.status)); 
            cli.close(); return cli; 
        }

        if(!cli.headers["Sec-Websocket-Accept"].empty() ){

            string_t dta = cli.headers["Sec-Websocket-Accept"];
            string_t sec = key + SECRET;

                auto sha = crypto::hash::SHA1();
                auto b64 = crypto::encode::BASE64(); 

                     sha.update(key);
                     b64.update( crypto::hex2buff(sha.get()) );
                     
                auto enc = b64.get().slice(0,-1);

        if( dta != enc ){ _ERROR("secret key does not match"); }   
            cli.stop();
        }   return cli;

    }

}
#undef NODEPP_GENERATOR
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_SWS) && ( defined(NODEPP_WS) || defined(NODEPP_WSS) )
    #define  GENERATOR_SWS
    #include "encoder.h"
namespace nodepp {

    struct ws_frame_t {
        bool  FIN = 1; //1b
        uint  RSV = 0; //3b
        uint  OPC = 1; //4b
        bool  MSK = 1; //1b
        char  KEY [4]; //4B
        ulong LEN = 0; //64b
    };

    /*─······································································─*/

    string_t write_ws_frame( char* /*unused*/, const ulong& sx ){

        auto bfx = ptr_t<char>( 64, '\0' ); uint idx = 0;
        auto byt = encoder::bytes::get( sx ); 
        bfx[idx] = (char) 0b10000001; idx++ ;

        if ( sx < 126 ){ 
            bfx[idx] = (uchar)(byt[byt.size()-1]); idx++;
        } elif ( sx < 65536 ){ 
            bfx[idx] = (uchar)( 126 ); idx++;
            bfx[idx] = (uchar)(byt[byt.size()-2]); idx++;
            bfx[idx] = (uchar)(byt[byt.size()-1]); idx++;
        } else {
            bfx[idx] = (uchar)( 127 ); idx++;
            bfx[idx] = (uchar)(byt[byt.size()-4]); idx++;
            bfx[idx] = (uchar)(byt[byt.size()-3]); idx++;
            bfx[idx] = (uchar)(byt[byt.size()-2]); idx++;
            bfx[idx] = (uchar)(byt[byt.size()-1]); idx++;
        }

        return { &bfx, idx }; 
    }

    int read_ws_frame( char* bf, ws_frame_t& st, int& _state_, ulong& size ){
        array_t<bool> y;
    gnStart st = ws_frame_t();
        
        y = array_t<bool>(encoder::bin::get( bf[0] )); 

        st.FIN    =   y.splice(0,1)[0] == 1;
        for( auto x : y.splice(0,3) ) st.RSV = st.RSV<<1 | x;
        for( auto x : y.splice(0,4) ) st.OPC = st.OPC<<1 | x;
        
        y = array_t<bool>(encoder::bin::get( bf[1] ));

        st.MSK    =   y.splice(0,1)[0] == 1;
        for( auto x : y.splice(0,7) ) st.LEN = st.LEN<<1 | x;

        if ( st.LEN  > 125 ){ 
        if ( st.LEN == 126 ){ size = 2; st.LEN = 0; }
        if ( st.LEN == 127 ){ size = 4; st.LEN = 0; } coNext;
        for( ulong x=0; x < size; x++ )
           { st.LEN = st.LEN << 8 | (uchar) bf[x]; }
        }

        if ( st.MSK == 1 ){ size=4; coNext; 
        for( ulong x=0; x < size; x++ )
           { st.KEY[x] = bf[x]; }
        }

    gnStop
    }

    /*─······································································─*/

namespace _ws_ {

    GENERATOR( read ){ 
    private:

        ws_frame_t frame;
        int        key=0;
        ulong      len=0;
        int        pos=0;

    public:
    
        int        state = 1;
        int        input = 0;
        int        output= 0;
        ulong      size  = 2;

    gnEmit( char* bf, const ulong& sx ) { 
        if( input<=0 ){ output=0; return -1; }
    gnStart key=0; output=-2; len=0; size=2; pos=0;

        while( read_ws_frame( bf, frame, pos, size )==1 )
             { coSet(1); return -1; coYield(1); }
        
        /*------*/

        if( frame.LEN ==  0 ){ size = 2; input = 0; coGoto(0); }
        if( frame.OPC >= 20 ){ state=-1; output=-1; coEnd; }
        if( frame.OPC ==  8 ){ state=-1; output=-1; coEnd; } 

        /*------*/

        size = frame.LEN; coSet(2); return -1; coYield(2);

        /*------*/

        for( int x=0; x<input && frame.MSK ; x++ )
           { bf[x] = bf[x] ^ frame.KEY[key]; key++; key%=4; }

             len += input; output = input;
             size-= input; input  = 0;

        if ( size == 0 ){ size = 2; input = 0; coGoto(0); }

        /*------*/

        coGoto(2);
    gnStop
    }};

    GENERATOR( write ){
    private:

        string_t   brr;
        string_t   hdr;

    public:
    
        int        state = 1;
        int        input = 0;
        int        output= 0;
        ulong      size  = 0;

    gnEmit( char* bf, const ulong& sx ) {
    gnStart size=sx; output=0; input=0;

        /*------*/

        hdr = write_ws_frame( bf, size ); 
        brr = string_t      ( bf, size ); 

        /*------*/

        size=hdr.size();input=0;output=0;
        memmove( bf, hdr.data( ), size ); 

        /*------*/

        do{ if( input > 0 ){
            output += input; size -= input;
        if( size == 0 ){ break; }
            memmove( bf,bf+input,sx-input );
        }   coSet(1); return -1; coYield(1); 
        }   while(1);

        /*------*/

        size=brr.size();input=0;output=0;
        memmove( bf, brr.data( ), size ); 

        /*------*/

        do{ if( input > 0 ){
            output += input; size -= input;
        if( size == 0 ){ break; }
            memmove( bf,bf+input,sx-input );
        }   coSet(2); return -1; coYield(2); 
        }   while(1);

        /*------*/

        coGoto(0);
    gnStop
    }};

}}
#endif
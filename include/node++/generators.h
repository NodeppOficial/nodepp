#if !defined(GENERATOR_FILE) && defined(NODEPP_STREAM)
    #define  GENERATOR_FILE
namespace nodepp { namespace _file_ {

    $Generator( read ){ public: 

        ulong*   r; int c;
        string_t y; 
        ulong dist;

    template< class T > $Emit( T* str, ulong size=CHUNK_SIZE ){
    $Start c=0; y.clear(); y = str->get_borrow(); str->del_borrow(); str->flush();

        if( !str->is_available() ){ str->close(); $End; } r = str->get_range();

        if( r[1] != 0 ){ auto pos = str->pos(); dist = r[1]-r[0];
             if( pos < r[0] ){ str->del_borrow(); str->pos( r[0] ); }
        else if( pos >=r[1] ){ str->close(); $End; }
        } else { dist = str->get_buffer_size(); }

        do{ if( c==0 && !y.empty() ){ break; } if( c==-2 ){ $Next; }
         ulong act = clamp( act, size, dist ); 
                 c = str->_read( str->get_buffer_data(), act );
        } while( c == -2 );

        if( c<=0 && y.empty() ){ str->close(); $End; } else if( c>0 ){
            y = (string_t){ str->get_buffer_data(), (ulong) c };
        }   c = y.size();

    $Stop
    }};
    
    /*─······································································─*/

    $Generator( write ){ public:

        ulong    y; 
        int      c;
        ulong size;
        
    template< class T > $Emit( T* str, const string_t& msg ){
    $Start c=0; y=0; str->flush();

        if(!str->is_available() || msg.empty() )
          { str->close(); $End; } 

        if( str->get_borrow().empty() ){ str->set_borrow(msg); }

        do{ while(( c=str->_write( str->get_borrow_data(), str->get_borrow_size() ))==-2 )
                 { $Next; } str->get_borrow().splice(0,c); y+=c;
        }   while( c>=0 && !str->get_borrow().empty() ); 

               if( c<=0 ){ str->close(); $End; }

    $Stop
    }};

    /*─······································································─*/

    $Generator( line ){ public: 
    
        _file_::read prs;
        string_t s,y;  
        ulong      c; 

    template< class T > $Emit( T* str ){
    $Start s.clear(); y.clear(); c=1;

        while( str->is_available() ){
        while( prs(str) == 1 ){ $Next; }
           if( prs.c<=0 ){ break; } c=1; s += prs.y; 
          for( auto x:s ){ if( x == '\n' ){ break; } c++; }
           if( c<=s.size() ){ break; }
        }      str->set_borrow(s);
        
        y = str->get_borrow().splice( 0, c );
    
    $Stop
    }};

}}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_ENCODE) && defined(NODEPP_ENCODE)
    #define  GENERATOR_ENCODE 
namespace nodepp { namespace _encode_ {

    _file_::write _write;
    _file_::read  _read;

    char *ibff, *obfp; 
    ulong ibfz, obfz; 
    ulong result;

    char     obff;
    string_t data;
    iconv_t  ctx;

    $Generator(pipe) { public:
        
        template< class T, class V >
        $Emit( const string_t& from, const string_t& to, const T& inp, const V& out, ulong mult=1 ){
        $Start  ctx = iconv_open( to.data(), from.data() ); 

            if( ctx == (iconv_t)-1 ){
                $onError(inp.onError,"can't open a encode context");
                $onError(out.onError,"can't open a encode context"); $End;
            }

            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 ){ $Next; }
               if( _read.c <= 0 )  { break; }

                obff = ptr_t<char>( _read.y.size() * mult );
                ibff = data.data(); ibfz = _read.y.size();
                obfz = obff.size(); obfp = &obff;
                
                result = iconv( ctx, (char**)&ibff, &ibfz, &obfp, &obfz );

                if( result == (ulong)-1 ){

                    if( errno == EINVAL ) {
                        string_t message = "Input conversion stopped due to an incomplete character or shift sequence at the end of the input buffer.";
                        $onError(inp.onError,message);
                        $onError(out.onError,message); break;
                    }

                    else if( errno == EILSEQ ) {
                        string_t message = "Input conversion stopped due to an input byte that does not belong to the input codeset.";
                        $onError(inp.onError,message);
                        $onError(out.onError,message); break;
                    }

                    else if( errno == E2BIG ) {
                        string_t message = "Input conversion stopped due to lack of space in the output buffer.";
                        $onError(inp.onError,message);
                        $onError(out.onError,message); break;
                    }

                    else if( errno == EBADF ) {
                        string_t message = "The cd argument is not a valid open conversion descriptor";
                        $onError(inp.onError,message);
                        $onError(out.onError,message); break;
                    }

                    else {
                        string_t message = "can't encode correctly";
                        $onError(inp.onError,message);
                        $onError(out.onError,message); break;
                    }

                } else {
                    string_t info = (string_t){ &obff, obff.size() }; 
                    inp.onData.emit( info ); out.write( info );
                }

            }   iconv_close(ctx); out.close(); inp.close();

        $Stop
        }
        
        template< class T >
        $Emit( string_t from, string_t to, const T& inp, ulong mult=1 ){
        $Start  ctx = iconv_open( to.data(), from.data() ); 

            if( ctx == (iconv_t)-1 ){
                $onError(inp.onError,"can't open a encode context"); $End;
            }

            while( inp.is_available() ){
            while( _read(&inp)==1 ){ $Next; }
               if( _read.c <= 0 )  { break; }

                obff = ptr_t<char>( _read.y.size() * mult );
                ibff = data.data(); ibfz = _read.y.size();
                obfz = obff.size(); obfp = &obff;
                
                result = iconv( ctx, (char**)&ibff, &ibfz, &obfp, &obfz );

                if( result == (ulong)-1 ){

                    if( errno == EINVAL ) {
                        string_t message = "Input conversion stopped due to an incomplete character or shift sequence at the end of the input buffer.";
                        $onError(inp.onError,message); break;
                    }

                    else if( errno == EILSEQ ) {
                        string_t message = "Input conversion stopped due to an input byte that does not belong to the input codeset.";
                        $onError(inp.onError,message); break;
                    }

                    else if( errno == E2BIG ) {
                        string_t message = "Input conversion stopped due to lack of space in the output buffer.";
                        $onError(inp.onError,message); break;
                    }

                    else if( errno == EBADF ) {
                        string_t message = "The cd argument is not a valid open conversion descriptor";
                        $onError(inp.onError,message); break;
                    }

                    else {
                        string_t message = "can't encode correctly";
                        $onError(inp.onError,message); break;
                    }

                } else {
                    string_t info = (string_t){ &obff, obff.size() }; 
                    inp.onData.emit( info ); 
                }

            }   iconv_close(ctx); inp.close();

        $Stop
        }

    };

}}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_ZLIB) && defined(NODEPP_ZLIB)
    #define  GENERATOR_ZLIB 
namespace nodepp { namespace _zlib_ {

    $Generator( inflate ){ public:
    
        ptr_t<z_stream> str = new z_stream;
        int x=0; ulong size; string_t dout;
        _file_::write _write;
        _file_::read  _read;
        

        template< class T, class V, class U >
        $Emit( const T& inp, const V& out, const U& cb ){
        $Start inp.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                $onError( inp.onError, message );
                $onError( inp.onError, message ); $End;
            }

            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 ){ $Next; }
               if( _read.c <= 0 )  { break; }

                str->avail_in = _read.y.size();
                str->avail_out= inp.get_buffer_size();
                str->next_in  = (Bytef*)_read.y.data();
                str->next_out = (Bytef*)inp.get_buffer_data(); 
                            x = ::inflate( &str, Z_FINISH );

                if(( size=inp.get_buffer_size()-str->avail_out )>0){
                    dout = (string_t){ inp.get_buffer_data(), size };
                    inp.onData.emit(dout); 
                    while( _write(&out,dout)==1 ){ $Next; } continue;
                }
                
                if( x==Z_STREAM_END ) { break; } else if( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    $onError( inp.onError, message );
                    $onError( out.onError, message ); break;
                }
            
            }   inflateEnd( &str ); out.close(); inp.close(); $Stop

        }

        template< class T, class U >
        $Emit( const T& inp, const U& cb ){
        $Start inp.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                $onError( inp.onError, message ); $End;
            }

            while( inp.is_available() ){
            while( _read(&inp)==1 ){ $Next; }
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

                if( x==Z_STREAM_END ) { break; } else if( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    $onError( inp.onError, message ); inp.close(); break;
                } 

            }   inflateEnd( &str ); inp.close(); $Stop

        }

    };
    
    /*─······································································─*/

    $Generator( deflate ){ public:
    
        ptr_t<z_stream> str = new z_stream;
        int x=0; ulong size; string_t dout;
        _file_::write _write;
        _file_::read  _read;
        

        template< class T, class V, class U >
        $Emit( const T& inp, const V& out, const U& cb ){
        $Start inp.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                $onError( inp.onError, message );
                $onError( inp.onError, message ); $End;
            }

            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 ){ $Next; }
               if( _read.c <= 0 )  { break; }

                str->avail_in = _read.y.size();
                str->avail_out= inp.get_buffer_size();
                str->next_in  = (Bytef*)_read.y.data();
                str->next_out = (Bytef*)inp.get_buffer_data(); 
                            x = ::deflate( &str, Z_PARTIAL_FLUSH );

                if(( size=inp.get_buffer_size()-str->avail_out )>0){
                    dout = (string_t){ inp.get_buffer_data(), size };
                    inp.onData.emit(dout); 
                    while( _write(&out,dout)==1 ){ $Next; } continue;
                }

                if( x==Z_STREAM_END ) { break; } else if( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    $onError( inp.onError, message );
                    $onError( out.onError, message ); break;
                }
            
            }   deflateEnd( &str ); out.close(); inp.close(); $Stop

        }

        template< class T, class U >
        $Emit( const T& inp, const U& cb ){
        $Start inp.onPipe.emit();

            str->zfree    = Z_NULL;
            str->zalloc   = Z_NULL;
            str->opaque   = Z_NULL;
            str->next_in  = Z_NULL;
            str->avail_in = Z_NULL;

            if( cb( &str ) != Z_OK ){ 
                string_t message = "Failed to initialize zlib for compression.";
                $onError( inp.onError, message ); $End;
            }

            while( inp.is_available() ){
            while( _read(&inp)==1 ){ $Next; }
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
                
                if( x==Z_STREAM_END ) { break; } else if( x < 0 ){ 
                    string_t message = string::format("ZLIB: %s",str->msg);
                    $onError( inp.onError, message ); inp.close(); break;
                } 

            }   deflateEnd( &str ); inp.close(); $Stop

        }

    };

}}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_STREAM) && defined(NODEPP_STREAM)
    #define  GENERATOR_STREAM 
namespace nodepp { namespace _stream_ {

    $Generator( pipe ){ public:

        _file_::write _write;
        _file_::read  _read;

        template< class T > $Emit( const T& inp ){
        $Start inp.onPipe.emit();
            while( inp.is_available() ){
            while( _read(&inp)==1 ){ $Next; } 
               if( _read.c <= 0 )  { break; }
                inp.onData.emit( _read.y );
            }   inp.close();
        $Stop
        }

        template< class T, class V > $Emit( const T& inp, const V& out ){
        $Start inp.onPipe.emit();
            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 )         { $Next; } 
               if( _read.c <= 0 )           { break; }
                inp.onData.emit( _read.y );
            while( _write(&out,_read.y)==1 ){ $Next; }
            }   inp.close(); out.close();
        $Stop
        }

    };
    
    /*─······································································─*/

    $Generator( line ){ public:

        _file_::write _write;
        _file_::line  _read;

        template< class T > $Emit( const T& inp ){
        $Start inp.onPipe.emit();
            while( inp.is_available() ){
            while( _read(&inp)==1 ){ $Next; } 
               if( _read.c <= 0 )  { break; }
                inp.onData.emit( _read.y );
            }   inp.close();
        $Stop
        }

        template< class T, class V > $Emit( const T& inp, const V& out ){
        $Start inp.onPipe.emit();
            while( inp.is_available() && out.is_available() ){
            while( _read(&inp)==1 )         { $Next; } 
               if( _read.c <= 0 )           { break; }
                inp.onData.emit( _read.y );
            while( _write(&out,_read.y)==1 ){ $Next; }
            }   inp.close(); out.close();
        $Stop
        }

    };
    
}}
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#if !defined(GENERATOR_TIMER) && defined(NODEPP_TIMER)
    #define  GENERATOR_TIMER
namespace nodepp { namespace _timer_ {

    $Generator( timer ){ public:

        template< class V, class... T > 
        $Emit( const V& func, const ptr_t<ulong>& out, ulong time, const T&... args ){
        $Start
            if(*out == 0 )                    $End;
            if( process::millis() <= *out )  $Goto(0);
            if ( func(args...)<0 )            $End;
            *out = process::millis() + time; $Goto(0); 
        $Stop
        }

        template< class V, class... T > 
        $Emit( const V& func, const ptr_t<ulong>& out, ulong* time, const T&... args ){
        $Start
            do { if(*out == 0 ) {  $End; } $Next;
               } while( process::millis() <= *out ); 
            if ( func(args...)<0 ){ $End; }
            *out = process::millis() + *time; $Goto(0); 
        $Stop
        }

    };
    
    /*─······································································─*/

    $Generator( utimer ){ public:

        template< class V, class... T > 
        $Emit( const V& func, const ptr_t<ulong>& out, ulong time, const T&... args ){
        $Start
            do { if(*out == 0 ) {  $End; } $Next;
               } while( process::micros() <= *out ); 
            if ( func(args...)<0 ){ $End; }
            *out = process::micros() + time; $Goto(0); 
        $Stop
        }

        template< class V, class... T > 
        $Emit( const V& func, const ptr_t<ulong>& out, ulong* time, const T&... args ){
        $Start
            do { if(*out == 0 ) {  $End; } $Next;
               } while( process::micros() <= *out ); 
            if ( func(args...)<0 ){ $End; } 
            *out = process::micros() + *time; $Goto(0);
        $Stop
        }

    };

}}
#endif

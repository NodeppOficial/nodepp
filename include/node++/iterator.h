#ifndef NODEPP_ITERATOR
#define NODEPP_ITERATOR

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace string {

    template< class U, class T >
    void map( const U& func, const T& argc ){ func( string::to_string(argc) ); }

    template< class U, class T, class... V > 
    void map( const U& func, const T& argc, const V&... args ){ func( string::to_string(argc) ); string::map( func, args... ); }

    /*─······································································─*/

    template< class U, class T, class... V > 
    long count( const U& func, const T& argc, const V&... args ){ ulong n = 0;
	    string::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); return n;
    }

    template< class U, class T, class... V > 
    T reduce( const U& func, const T& argc, const V&... args ){ T act = argc;
	    string::map([&]( T argc ){ act = func( act, argc ); }, args... ); return act;
    }
    
    /*─······································································─*/

    template< class U, class T, class... V > 
    bool every( const U& func, const T& argc, const V&... args ){ ulong n = 0;
	    string::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n == (sizeof...(V)+1) );
    }

    template< class U, class T, class... V > 
    bool some( const U& func, const T& argc, const V&... args ){ ulong n = 0;
	    string::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n > 0 );
    }

    template< class U, class T, class... V > 
    bool none( const U& func, const T& argc, const V&... args ){ ulong n = 0;
	    string::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n == 0 );
    }

    /*─······································································─*/
#ifndef ARDUINO
    template< class T, class... V > 
    T max( const T& argc, const V&... args ){ T n = argc;
	    string::map([&]( T argc ){ if( argc > n ) n=argc; }, args... );
        return n;
    }

    template< class T, class... V > 
    T min( const T& argc, const V&... args ){ T n = argc;
	    string::map([&]( T argc ){ if( argc < n ) n=argc; }, args... );
        return n;
    }
#endif
    /*─······································································─*/

    template< class T, class... V >
    string_t join( const string_t& c, const T& argc, const V&... args ){
	array_t<string_t> list; string_t result; 
        map([&]( string_t argc ){ list.push( argc ); }, argc, args... );
            for( ulong x=0; x<list.size(); x++ ){
            result += to_string(list[x]) + ((x==list.last()) ? "" : c);
        }   return result;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace iterator {

    template< class U, class T >
    void map( const U& func, const T& argc ){ func( argc ); }

    template< class U, class T, class... V > 
    void map( const U& func, const T& argc, const V&... args ){ func( argc ); iterator::map( func, args... ); }

    /*─······································································─*/

    template< class U, class T, class... V > 
    long count( const U& func, const T& argc, const V&... args ){ ulong n = 0;
	    iterator::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); return n;
    }

    template< class U, class T, class... V > 
    T reduce( const U& func, const T& argc, const V&... args ){ T act = argc;
	    iterator::map([&]( T argc ){ act = func( act, argc ); }, args... ); return act;
    }
    
    /*─······································································─*/

    template< class U, class T, class... V > 
    bool every( const U& func, const T& argc, const V&... args ){ ulong n = 0;
	    iterator::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n == (sizeof...(V)+1) );
    }

    template< class U, class T, class... V > 
    bool some( const U& func, const T& argc, const V&... args ){ ulong n = 0;
	    iterator::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n > 0 );
    }

    template< class U, class T, class... V > 
    bool none( const U& func, const T& argc, const V&... args ){ ulong n = 0;
	    iterator::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n == 0 );
    }

    /*─······································································─*/
#ifndef ARDUINO
    template< class T, class... V > 
    T max( const T& argc, const V&... args ){ T n = argc;
	    iterator::map([&]( T argc ){ if( argc > n ) n=argc; }, args... );
        return n;
    }

    template< class T, class... V > 
    T min( const T& argc, const V&... args ){ T n = argc;
	    iterator::map([&]( T argc ){ if( argc < n ) n=argc; }, args... );
        return n;
    }
#endif
    /*─······································································─*/

    template< class T, class... V >
    string_t join( const string_t& c, const T& argc, const V&... args ){
	    return string::join( c, argc, args... );
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
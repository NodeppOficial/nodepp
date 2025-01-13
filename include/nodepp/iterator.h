/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_ITERATOR
#define NODEPP_ITERATOR

namespace nodepp { namespace iterator {

    template< class U >
    void map( U func ){ }

    template< class U, class T >
    void map( U func, const T& argc ){ func( argc ); }

    template< class U, class T, class... V > 
    void map( U func, const T& argc, const V&... args ){ func( argc ); iterator::map( func, args... ); }

    /*─······································································─*/

    template< class U > 
    long count( U func ){ return 0; }

    template< class U, class T, class... V > 
    long count( U func, const T& argc, const V&... args ){ ulong n = 0;
	    iterator::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); return n;
    }

    template< class U, class T, class... V > 
    T reduce( U func, const T& argc, const V&... args ){ T act = argc;
	    iterator::map([&]( T argc ){ act = func( act, argc ); }, args... ); return act;
    }
    
    /*─······································································─*/

    template< class U > 
    bool every( U func ){ return false; }

    template< class U, class T, class... V > 
    bool every( U func, const T& argc, const V&... args ){ ulong n = 0;
	    iterator::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n == (sizeof...(V)+1) );
    }

    template< class U > 
    bool some( U func ){ return false; }

    template< class U, class T, class... V > 
    bool some( U func, const T& argc, const V&... args ){ ulong n = 0;
	    iterator::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n > 0 );
    }

    template< class U > 
    bool none( U func ){ return false; }

    template< class U, class T, class... V > 
    bool none( U func, const T& argc, const V&... args ){ ulong n = 0;
	    iterator::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n == 0 );
    }
    
}}

#endif

#if !defined(NODEPP_ITERATOR_STRING) && defined(NODEPP_STRING)
    #define  NODEPP_ITERATOR_STRING

namespace nodepp { namespace string {

    template< class U >
    void map( U func ){ }

    template< class U, class T >
    void map( U func, const T& argc ){ func( string::to_string(argc) ); }

    template< class U, class T, class... V > 
    void map( U func, const T& argc, const V&... args ){ func( string::to_string(argc) ); string::map( func, args... ); }

    /*─······································································─*/

    template< class U > 
    long count( U func ){ return 0; }

    template< class U, class T, class... V > 
    long count( U func, const T& argc, const V&... args ){ ulong n = 0;
	    string::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); return n;
    }

    template< class U, class T, class... V > 
    T reduce( U func, const T& argc, const V&... args ){ T act = argc;
	    string::map([&]( T argc ){ act = func( act, argc ); }, args... ); return act;
    }
    
    /*─······································································─*/

    template< class U > 
    bool every( U func ){ return false; }

    template< class U, class T, class... V > 
    bool every( U func, const T& argc, const V&... args ){ ulong n = 0;
	    string::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n == (sizeof...(V)+1) );
    }

    template< class U > 
    bool some( U func ){ return false; }

    template< class U, class T, class... V > 
    bool some( U func, const T& argc, const V&... args ){ ulong n = 0;
	    string::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n > 0 );
    }

    template< class U > 
    bool none( U func ){ return false; }

    template< class U, class T, class... V > 
    bool none( U func, const T& argc, const V&... args ){ ulong n = 0;
	    string::map([&]( T argc ){ if( func(argc) ) n++; }, argc, args... ); 
	    return ( n == 0 );
    }
    
    /*─······································································─*/

    template< class T >
    string_t join( const string_t& c ){ return nullptr; }

    template< class T, class... V >
    string_t join( const string_t& c, const T& argc, const V&... args ){
        array_t<string_t> list; string_t result; 
            map([&]( string_t argc ){ list.push( argc ); }, argc, args... );
                for( ulong x=0; x<list.size(); x++ ){
                result += to_string(list[x]) + ((x==list.last()) ? "" : c);
            }   return result;
    }

}}

#endif
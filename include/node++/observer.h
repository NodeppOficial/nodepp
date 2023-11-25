#ifndef NODEPP_OBSERVER
#define NODEPP_OBSERVER

/*────────────────────────────────────────────────────────────────────────────*/

#include "event.h" 
#include "type.h"
#include "any.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class observer_t {
private:

    template< class A, class B, class C >
    struct NODE_ { A first; B second; C third; };

    using V = any_t;
    using U = string_t; 
    using E = event_t<V,V>;
    using T = NODE_<U,V,E>;

protected: 
    array_t<T> node;

public:

    template< class... O >
    observer_t( const T& argc, const O&... args ) noexcept {
        node = array_t<T>( (ulong) sizeof...(args)+1 );
        ulong x = 0; iterator::map([&]( T arg ){
            node[x] = arg; x++;
        }, argc, args... );
    }
    
    /*─······································································─*/

    template< ulong N >
    observer_t& operator=( const T (&args) [N] ) noexcept {
        node = array_t<T>( N ); 
        for( ulong x=N; x--; )
           { node[x] = args[x]; }
             return *this;
    }

    template< ulong N >
    observer_t( const T (&args) [N] ) noexcept { 
        node = array_t<T>( N ); 
        for( ulong x=N; x--; )
           { node[x] = args[x]; }
    }
    
    /*─······································································─*/

    template< class F >
    ulong once( U name, F func ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
            if( node[x].first == name )
                return node[x].third.once( func );
        }       return 0;
    }

    template< class F >
    ulong on( U name, F func ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
            if( node[x].first == name )
                return node[x].third.on( func );
        }       return 0;
    }

    void off( U name, ulong id ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
            if( node[x].first == name )
                node[x].third.off( id );
        }
    }
    
    /*─······································································─*/
    
    void set( function_t<observer_t,observer_t> func ) const noexcept {
        observer_t obj = func( *this ); for( auto x : obj )
            this->set( x.first, x.second );
    }
    
    /*─······································································─*/

    template< class F >
    void set( U name, F value ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
            if( node[x].first == name ){
                node[x].third.emit( node[x].second, value );
                node[x].second = value;
            }
        }
    }
    
    /*─······································································─*/
    
    template< class V, ulong N >
    void set( const V (&args) [N] ) const noexcept {
        for( ulong x=0; x<N; x++ )
             this->set( args[x].first, args[x].second );
    }

    /*─······································································─*/

    const V get( U name ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
            if( node[x].first == name )
                return node[x].second;
        }       return ( const V ){0};
    }

    /*─······································································─*/

    const T* begin() const noexcept { return node.begin(); }
    const T*   end() const noexcept { return node.end();   }
    
    /*─······································································─*/

    const V operator[]( U name ) const noexcept {
        
        for( ulong x=0; x<node.size(); x++ ){
            if( node[x].first == name )
            return node[x].second;
        }   
        
        return ( const V ){ 0 };
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
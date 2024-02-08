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
    using P = type::pair<U,V>;

protected: array_t<T> node;
public:    observer_t() noexcept {}

    template< class... O >
    observer_t( const P& argc, const O&... args ) noexcept {
        node = array_t<T>( (ulong) sizeof...(args)+1 );
        ulong x = 0; iterator::map([&]( P arg ){
            node[x].second = arg.second;
            node[x].first  = arg.first;  x++;
        }, argc, args... );
    }
    
    /*─······································································─*/

    template< ulong N >
    observer_t& operator=( const P (&args) [N] ) noexcept {
        node = array_t<T>( N ); for( ulong x=N; x--; ){ 
        node[x].second = args[x].second; 
        node[x].first  = args[x].first;
        } return *this;
    }

    template< ulong N >
    observer_t( const P (&args) [N] ) noexcept { 
        node = array_t<T>( N ); for( ulong x=N; x--; ){ 
        node[x].second = args[x].second;
        node[x].first = args[x].first; 
        }
    }
    
    /*─······································································─*/

    template< class F >
    void* once( const U& name, F func ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
            if( node[x].first == name )
                return node[x].third.once( func );
        }       return 0;
    }

    template< class F >
    void* on( const U& name, F func ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
            if( node[x].first == name )
                return node[x].third.on( func );
        }       return 0;
    }

    void off( const U& name, void* id ) const noexcept {
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
    void set( const U& name, const F& value ) const noexcept {
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

    const V get( const U& name ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
            if( node[x].first == name )
                return node[x].second;
        }       return (const V)(0);
    }

    /*─······································································─*/

    const T* begin() const noexcept { return node.begin(); }
    const T*   end() const noexcept { return node.end();   }
    
    /*─······································································─*/

    const V operator[]( const U& name ) const noexcept {
        for( ulong x=0; x<node.size(); x++ ){
            if( node[x].first == name )
            return node[x].second;
        }   return (const V)(0);
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
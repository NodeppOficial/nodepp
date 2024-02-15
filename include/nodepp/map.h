#ifndef NODEPP_MAP
#define NODEPP_MAP

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template<class U, class V>
class map_t { 
protected:

    using T = type::pair<U,V>;

    struct NODE {
        queue_t<T> queue;
    };  ptr_t<NODE> obj;

public: 

    template< class... O >
    map_t( const T& argc, const O&... args ) noexcept : obj(new NODE()) {
        iterator::map([&]( T arg ){ obj->queue.push(arg); }, argc, args... );
    }

    template< ulong N >
    map_t& operator=( const T (&args) [N] ) noexcept {
        for( auto &x: args ){ obj->queue.push(x); } return *this;
    }

    template< ulong N >
    map_t( const T (&args) [N] ) noexcept : obj(new NODE()) { 
        for( auto &x: args ){ obj->queue.push(x); }
    }
    
    map_t() noexcept : obj(new NODE()) {}

    /*─······································································─*/

    V& operator[]( const U& id ) const noexcept { 
        auto x = obj->queue.first(); 
        
        while( !id.empty() && x != nullptr ){
            if ( x->data.first == id )
               { return x->data.second; } 
            else x = x->next; 
        }

               obj->queue.push({ id, V() }); 
        return obj->queue.last()->data.second;
    }

    /*─······································································─*/

    ptr_t<T> data() const noexcept { return obj->queue.data(); }
    
    /*─······································································─*/

    void erase() const noexcept { obj->queue.erase(); }

    void erase( const U& id ) const noexcept {
         obj->queue.erase( obj->queue.index_of([&]( T arg ){ return arg.first == id; }) );
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
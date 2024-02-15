#ifndef NODEPP_QUEUE
#define NODEPP_QUEUE

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {
template< class V > class queue_t {
protected: 

    class NODE { public:
        NODE* next = nullptr; 
        NODE* prev = nullptr; V data; 
        NODE( V value ){ data = value; } 
        explicit operator V(){ return data; }
    };

    NODE*       act = nullptr;
    ptr_t<NODE> queue;

    ptr_t<ulong> get_slice_range( long x, long y ) const noexcept {
        
        if( empty() || x == y ){ return nullptr; } if( y>0 ){ y--; }

        if( x < 0 ){ x = size()-1+x; } if( (ulong)x > size()-1 ){ return nullptr; }
        if( y < 0 ){ y = size()-1+y; } if( (ulong)y > size()-1 ){ y = size()-1; } 
                                       if( y < x )              { return nullptr; }

        ulong a = clamp( (ulong)y, 0UL, size()-1 );
        ulong b = clamp( (ulong)x, 0UL, a ); 
        ulong c = a - b + 1; return {{ b, a, c }};

    }

    ptr_t<ulong> get_splice_range( long x, ulong y ) const noexcept {
        
        if( empty() || y == 0 ){ return nullptr; }

        if( x < 0 ){ x = size()-1+x; } if( (ulong)x > size()-1 ){ return nullptr; }
            y += x - 1;
        if( y > size()-1 ){ y= size()-1; } if( y < (ulong)x ){ return nullptr; }

        ulong a = clamp( (ulong)y, 0UL, size()-1 );
        ulong b = clamp( (ulong)x, 0UL, a ); 
        ulong c = a - b + 1; return {{ b, a, c }};

    }
    
public: queue_t() noexcept {} 

    ptr_t<NODE>& ptr() noexcept { return queue; }

    virtual ~queue_t() noexcept { 
        if( queue.count() > 1 )
          { return; } clear(); 
    }
    
    /*─······································································─*/

    template< class T, ulong N >
    queue_t& operator=( const T (&value) [N] ) noexcept {
        NODE* n = &queue; for( ulong i=0; i<N; i++ ){ 
            if( n == nullptr ){ 
                queue = new NODE( value[i] ); 
                    n = &queue; 
            } else {
                n->next = new NODE( value[i] );
                n->next->prev = n; n = n->next;
            }
        }   return *this;
    }

    template < class T, ulong N >
    queue_t( const T (&value)[N] ) noexcept { 
        NODE* n = &queue; for( ulong i=0; i<N; i++ ){ 
            if( n == nullptr ){ 
                queue = new NODE( value[i] ); 
                    n = &queue; 
            } else {
                n->next = new NODE( value[i] );
                n->next->prev = n; n = n->next;
            }
        }
    }
    
    /*─······································································─*/

    template< ulong N >
    queue_t& operator=( const V (&value) [N] ) noexcept {
        NODE* n = &queue; for( ulong i=0; i<N; i++ ){ 
            if( n == nullptr ){ 
                queue = new NODE( value[i] ); 
                    n = &queue; 
            } else {
                n->next = new NODE( value[i] );
                n->next->prev = n; n = n->next;
            }
        }   return *this;
    }

    template < ulong N >
    queue_t( const V (&value)[N] ) noexcept { 
        NODE* n = &queue; for( ulong i=0; i<N; i++ ){ 
            if( n == nullptr ){ 
                queue = new NODE( value[i] ); 
                    n = &queue; 
            } else {
                n->next = new NODE( value[i] );
                n->next->prev = n; n = n->next;
            }
        }
    }
    
    /*─······································································─*/

    template < class T >
    queue_t( const T* value, ulong N ) noexcept { 
        if( value == nullptr || N == 0 ){ return; }
        NODE* n = &queue; for( ulong i=0; i<N; i++ ){ 
            if( n == nullptr ){ 
                queue = new NODE( value[i] ); 
                    n = &queue; 
            } else {
                n->next = new NODE( value[i] );
                n->next->prev = n; n = n->next;
            }
        }
    }
    
    /*─······································································─*/

    bool empty() const noexcept { return queue == nullptr ? 1 : size() <= 0; }

    ulong size() const noexcept { 
           if( queue == nullptr ){ return 0; } NODE* n = &queue; ulong i = 1; 
        while( n->next != nullptr ){ n = n->next; i++; } return i;
    }
    
    /*─······································································─*/

    ptr_t<V> data() const noexcept { 
        if( empty() ){ return nullptr; } ptr_t<V> res ( size() );
        ulong y=0; auto x = first(); while( x != nullptr ){ 
            res[y] = x->data; x = x->next; y++;
        }   return res;
    }
    
    /*─······································································─*/

    NODE* operator&( void ) const noexcept { return act==nullptr ? first() : act; }

    template< class... T >
    NODE* operator[]( T... args ) noexcept { return this->get( args... ); }
    
    /*─······································································─*/

    long index_of( function_t<bool,V> func ) const noexcept {
        long i=0; NODE* n = first(); if( empty() ){ return -1; } 
        while( n!=nullptr ) { 
            if( func(n->data)== 1 ){ return i; }
            if( n->next == nullptr ){ break; }
            i++; n = n->next;
        }   return -1;
    }

    ulong count( function_t<bool,V> func ) const noexcept { 
        ulong i=0; NODE* n = first(); if( empty() ){ return 0; } 
        while( n!=nullptr ) { 
            if( func(n->data)== 1 ){ i++; }
            if( n->next == nullptr ){ break; }
                n = n->next;
        }   return i;
    }
    
    /*─······································································─*/

    bool some( function_t<bool,V> func ) const noexcept {
        if( empty() ){ return false; } NODE* n = first(); 
        while( n!=nullptr ) { 
            if( func(n->data)== 1 ){ return 1; }
            if( n->next == nullptr ){ break; }
                n = n->next;
        }   return 0;
    }

    bool none( function_t<bool,V> func ) const noexcept {
        if( empty() ) return false;
        NODE* n = first(); while( n!=nullptr ) { 
            if( func(n->data)== 1 ){ return 0; }
            if( n->next == nullptr ){ break; }
                n = n->next;
        }   return 1;
    }

    bool every( function_t<bool,V> func ) const noexcept {
        if( empty() ) return false;
        NODE* n = first(); while( n!=nullptr ) { 
            if( func(n->data)== 0 ){ return 0; }
            if( n->next == nullptr ){ break; }
                n = n->next;
        }   return 1;
    }

    void map( function_t<void,V> func ) const noexcept {
        if( empty() ){ return; } NODE* n = first(); 
        while( n!=nullptr ){ func( n->data ); n = n->next; }
    }

    /*─······································································─*/

#ifndef ARDUINO

    NODE* max() const noexcept { 
        if( empty() ){ return nullptr; } 
        NODE* n = first(), p = first(); while( n!=nullptr ){ 
            if( p->data < n->data ){ p = n; } n = n->next;
        }   return p;
    }

    NODE* min() const noexcept { 
        if( empty() ){ return nullptr; } 
        NODE* n = first(), p = first(); while( n!=nullptr ){ 
            if( p->data > n->data ){ p = n; } n = n->next;
        }   return p;
    }

#endif
    
    bool is_item( NODE* item ) const noexcept {
        auto n = first(); while( n != nullptr && item != nullptr ){
             if( n == item ){ return 1; } n = n->next;
        }    return 0;
    }

    /*─······································································─*/

    void unshift( const V& value ) noexcept { insert( first(), value ); }
    void    push( const V& value ) noexcept { insert( last(), value ); }
    void                   shift() noexcept { erase( first() ); }
    void                     pop() noexcept { erase( last() ); }
    
    /*─······································································─*/

    void clear() noexcept { while( !empty() ){ shift(); } }
    void erase() noexcept { while( !empty() ){ shift(); } }
    void  free() noexcept { while( !empty() ){ shift(); } }
    
    /*─······································································─*/

    void insert( ulong index, const V& value ) noexcept { 
	    index = clamp( index, 0UL, size() - 1 );
	    insert( get(index), value ); 
    }

    void insert( ulong index, V* value, ulong N ) noexcept {
	    index = clamp( index, 0UL, size() - 1 );
    	ulong i=index; for( ulong x=0; x<N; x++ ) {
	        insert( x, value[x] );
        }
    }

    template< ulong N >
    void insert( ulong index, const V(&value)[N] ) noexcept {
	    index = clamp( index, 0UL, size() - 1 );
    	ulong i=index; for( ulong x=0; x<N; x++ ) {
	        insert( x, value[x] );
        }
    }

    void insert( NODE* index, const V& value ) noexcept {
        if( empty() ){ queue = new NODE( value ); } 
        if( is_item(index) ){ if( index == last() ) {
                index->next = new NODE( value );
                index->next->prev = index;
            } elif ( index == first() ) {
                auto  prev  = *queue; queue = new NODE( value );
                queue->next = new NODE( prev );
                queue->next->prev = index;
            } else {
                auto prev = index->next;
                index->next = new NODE( value ); 
                index->next->prev = index;
                prev->prev = index->next;
                index->next->next = prev;
            }
        }
    }
    
    /*─······································································─*/

    void erase( ulong begin, ulong end ) noexcept {
        auto r = get_slice_range( begin, end );
           if( r == nullptr ){ return; }
        while( r[2]-->0 ) { erase( r[0] ); }
    }

    void erase( ulong begin ) noexcept { 
        auto r = get_slice_range( begin, size() );
           if( r == nullptr ){ return; }
        erase( get( r[0] ) ); 
    }

    void erase( NODE* x ) noexcept {
        if( x == nullptr || empty() ){ return; }
        if( x == act ){ next(); } if ( x == first() ) {
            if ( x->next != nullptr ){ x->next->prev = nullptr; }
                 x->prev  = nullptr; queue = x->next;
        } else {
            if ( x->prev != nullptr ){ x->prev->next = x->next; }
            if ( x->next != nullptr ){ x->next->prev = x->prev; } delete x;
        }
    }

    /*─······································································─*/

    void set( NODE* x ) noexcept { if ( is_item(x) ) act = x; }

    NODE* get() noexcept { return act==nullptr ? first() : act; }

    NODE* get( ulong x ) noexcept { 
        if( empty() ){ return nullptr; } auto n = first();
        while( n->next != nullptr && x-->0 ){ n = n->next; } return n;
    }
    
    /*─······································································─*/

    NODE* first() const noexcept { 
        return queue.null() ? nullptr : &queue; 
    }

    NODE* last() const noexcept {
        if( empty() ){ return nullptr; } NODE* n = &queue; 
        while( n->next != nullptr ){ n = n->next; } return n;
    }
    
    /*─······································································─*/
    
    NODE* prev() noexcept { 
        act = act != nullptr ? act->prev : last(); return act;
    }
    
    NODE* next() noexcept { 
        act = act != nullptr ? act->next : first(); return act;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
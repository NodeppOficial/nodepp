/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

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
    };
    
    NODE *act = nullptr, *lst = nullptr;
    ptr_t<ulong> length = new ulong(0);
    ptr_t<NODE>  queue;

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
        if( y > size()-1 ){ y= size()-1; } if( y < (ulong)x )   { return nullptr; }

        ulong a = clamp( (ulong)y, 0UL, size()-1 );
        ulong b = clamp( (ulong)x, 0UL, a ); 
        ulong c = a - b + 1; return {{ b, a, c }};

    }
    
public: queue_t() noexcept {}

    template< class T, ulong N >
    queue_t& operator=( const T (&value) [N] ) noexcept {
        auto n = &queue; *length = N; 
        auto i = N; while( i-->0 ){ 
            if( n == nullptr ){ 
                queue = new NODE( value[i] );
                lst = &queue; n = &queue;
            } else {
                n->next = new NODE( value[i] );
                n->next->prev = n;
                n = n->next; 
                lst = n;
            }
        }   return *this;
    }

    template < class T, ulong N >
    queue_t( const T (&value)[N] ) noexcept { 
        auto n = &queue; *length = N;
        auto i = N; while( i-->0 ){ 
            if( n == nullptr ){ 
                queue = new NODE( value[i] );
                lst = &queue; n = &queue;
            } else {
                n->next = new NODE( value[i] );
                n->next->prev = n;
                n = n->next; 
                lst = n;
            }
        }
    }
    
    /*─······································································─*/

    template < class T >
    queue_t( const T* value, ulong N ) noexcept { 
        if( value == nullptr || N == 0 ){ return; }
        auto n = &queue; *length = N;
        auto i = N; while( i-->0 ){ 
            if( n == nullptr ){ 
                queue = new NODE( value[i] );
                lst = &queue; n = &queue;
            } else {
                n->next = new NODE( value[i] );
                n->next->prev = n;
                n = n->next; 
                lst = n;
            }
        }
    }
    
    /*─······································································─*/

    queue_t<V> sort( function_t<bool,V,V> func ) noexcept {
        queue_t<V> n_buffer;

        auto x = this->first(); while( x != nullptr ){
        auto n = n_buffer.first();

            while( n!=nullptr ){ if( !func( x->data, n->data ) )
                 { n = n->next; continue; } break;
            }

            V item; memcpy( &item, &x->data, sizeof(V) );
            n_buffer.insert( n, item ); x = x->next;
        }

        return n_buffer;
    }
    
    /*─······································································─*/

    virtual ~queue_t() noexcept { if( queue.count() > 1 ){ return; } clear(); }
    
    /*─······································································─*/

    bool empty() const noexcept { return queue == nullptr ? 1 : size()==0; }

    ulong size() const noexcept { return queue == nullptr ? 0 : *length; }
    
    /*─······································································─*/

    NODE* operator[]( ulong idx ) noexcept { return this->get( idx ); }
    
    /*─······································································─*/

    ptr_t<V> data() const noexcept { 
        if( empty() ){ return nullptr; } ptr_t<V> res ( size() );
        ulong y=0; auto n = first(); while( n != nullptr ){ 
            res[y] = type::cast<V>( n->data ); 
            n = n->next; y++;
        }   return res;
    }
    
    /*─······································································─*/

    long index_of( function_t<bool,V> func ) const noexcept {
        long i=0; if( empty() ){ return -1; } 
        auto n = first(); while( n!=nullptr ) { 
            if( func(n->data) == 1 ){ return i; }
            if( n->next == nullptr ){ break; }
            i++; n = n->next;
        }   return -1;
    }

    ulong count( function_t<bool,V> func ) const noexcept { 
        ulong i=0; if( empty() ){ return 0; } 
        auto n = first(); while( n!=nullptr ) { 
            if( func(n->data) == 1 ){ i++; }
            if( n->next == nullptr ){ break; }
                n = n->next;
        }   return i;
    }
    
    /*─······································································─*/

    bool some( function_t<bool,V> func ) const noexcept {
        if( empty() ){ return false; } 
        auto n = first(); while( n!=nullptr ) { 
            if( func(n->data) == 1 ){ return 1; }
            if( n->next == nullptr ){ break; }
                n = n->next;
        }   return 0;
    }

    bool none( function_t<bool,V> func ) const noexcept {
        if( empty() ) return false;
        auto n = first(); while( n!=nullptr ) { 
            if( func(n->data) == 1 ){ return 0; }
            if( n->next == nullptr ){ break; }
                n = n->next;
        }   return 1;
    }

    bool every( function_t<bool,V> func ) const noexcept {
        if( empty() ) return false;
        auto n = first(); while( n!=nullptr ) { 
            if( func(n->data) == 0 ){ return 0; }
            if( n->next == nullptr ){ break; }
                n = n->next;
        }   return 1;
    }

    void map( function_t<void,V&> func ) const noexcept {
        if( empty() ){ return; } auto n = first(); 
        while( n!=nullptr ){ func( n->data ); n = n->next; }
    }

    /*─······································································─*/
    
    bool is_item( NODE* item ) const noexcept {
        auto n = first(); while( n!=nullptr ){
         if( n == item ){ return true; } 
             n = n->next;
        }    return false;
    }

    /*─······································································─*/

    void unshift( const V& value ) noexcept { insert( first(), value ); }
    void    push( const V& value ) noexcept { insert( nullptr, value ); }
    void                   shift() noexcept { erase( first() ); }
    void                     pop() noexcept { erase( nullptr ); }
    
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
    	ulong i=index; for( ulong x=0; x<N; x++ )
            { insert( x, value[x] ); }
    }

    template< ulong N >
    void insert( ulong index, const V(&value)[N] ) noexcept {
	    index = clamp( index, 0UL, size() - 1 );
    	ulong i=index; for( ulong x=0; x<N; x++ ) 
            { insert( x, value[x] ); }
    }

    void insert( NODE* n, const V& value ) noexcept {
        if( empty() ){
            queue = new NODE( value ); lst = &queue;
        } elif ( is_item(n) ) { 
            if ( n == first() ) {
                auto   prev = *queue; queue = new NODE( value );
                queue->next = new NODE( prev ); queue->next->prev =&queue;
                if( prev.next != nullptr ){ prev.next->prev = queue->next; }
                if( prev.next == nullptr ){ lst = queue->next; }
            } else {
                 auto   next = n->prev; n = new NODE( value );
            if ( next->next != nullptr ){ next->next->prev =n; }
                 n->next = next->next; n->prev = next;
            if ( n->next== nullptr ){ lst = n; }
                 next->next  = n;
            }
        } else { auto prev = last();
            prev->next = new NODE( value );
            prev->next->prev = prev;
            lst = prev->next;
        }  *length += 1;
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

    void erase( NODE* n ) noexcept {
        if( empty() )                { return; }
        if(!is_item(n) )             { n = last(); }
        if( n->next == nullptr )     { lst= n->prev; }
        if( n == act ){ next(); } if ( n == first() ) {
            if ( n->next != nullptr ){ n->next->prev = nullptr; }
                 n->prev = nullptr; queue = n->next;
        } else {
            if ( n->prev != nullptr ){ n->prev->next = n->next; }
            if ( n->next != nullptr ){ n->next->prev = n->prev; } 
                 delete n;
        }   *length -= 1; 
    }

    /*─······································································─*/

    void set( NODE* x ) noexcept { if ( is_item(x) ) act = x; }

    NODE* get() noexcept { return act==nullptr ? first() : act; }

    NODE* get( ulong x ) noexcept { 
        if( empty() ){ return nullptr; } auto n = first();
        while( n->next != nullptr && x-->0 ){ n = n->next; } return n;
    }
    
    /*─······································································─*/
    
    NODE* next() noexcept { act = act != nullptr ? act->next : first(); return act; }
    
    NODE* prev() noexcept { act = act != nullptr ? act->prev : last(); return act; }

    NODE* first() const noexcept { return queue == nullptr ? nullptr : &queue; }

    NODE* last()  const noexcept { return lst == nullptr ? first() : lst; }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

#ifndef NODEPP_ARRAY
#define NODEPP_ARRAY

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class T >
class array_t {    
protected: 

    ptr_t<T> buffer;

public:

    virtual ~array_t(){  }
    
    /*─······································································─*/

    array_t( T* value ) noexcept {
        if( value == nullptr ){ return; } ulong n = 0; 
        while( ( !value[n] || value[n] != '\0' ) && n < sizeof(value) ){ n++; }
        buffer = ptr_t<T>( value, n );
    }

    array_t( const ptr_t<T>& argc ) noexcept { buffer = argc; }

    array_t( const ulong& n, const T& c ) noexcept {
        if( n == 0 ){ return; } buffer = ptr_t<T>( n, c );
    }

    array_t( T* value, const ulong& n ) noexcept { 
        if( value == nullptr || n == 0 ){ return; } 
        buffer = ptr_t<T>( value, n );
    }

    array_t() noexcept = default;
    
    /*─······································································─*/

    template< class V, ulong N >
    array_t& operator=( const V (&value) [N] ) noexcept {
        ulong s = 0; buffer = ptr_t<T>( N ); 
        for( auto x=begin(); x!=end(); x++ )
           { (*x) = value[s]; s++; } return *this;
    }

    template < class V, ulong N > 
    array_t( const V (&value)[N] ) noexcept { 
        ulong s = 0; buffer = ptr_t<T>( N ); 
        for( auto x=begin(); x!=end(); x++ )
           { (*x) = value[s]; s++; } 
    }
    
    /*─······································································─*/

    template< ulong N >
    array_t& operator=( const T (&value) [N] ) noexcept {
        ulong s = 0; buffer = ptr_t<T>( N ); 
        for( auto x=begin(); x!=end(); x++ )
           { (*x) = value[s]; s++; } return *this;
    }

    template < ulong N > 
    array_t( const T (&value)[N] ) noexcept { 
        ulong s = 0; buffer = ptr_t<T>( N ); 
        for( auto x=begin(); x!=end(); x++ )
           { (*x) = value[s]; s++; } 
    }
    
    /*─······································································─*/

    T* end() const { return &buffer + size(); }
    T* begin() const { return &buffer; }
    
    /*─······································································─*/

    ulong first() const noexcept { return 0; }
    bool  empty() const noexcept { return buffer.empty(); }
    ulong  size() const noexcept { return empty() ? 0 : buffer.size() - 0; }
    ulong  last() const noexcept { return empty() ? 0 : buffer.size() - 1; }
    
    /*─······································································─*/

    bool operator> ( const array_t& oth ) const noexcept { return compare( oth ) == 1; }
    bool operator>=( const array_t& oth ) const noexcept { return compare( oth ) >= 0; }
    bool operator<=( const array_t& oth ) const noexcept { return compare( oth ) <= 0; }
    bool operator< ( const array_t& oth ) const noexcept { return compare( oth ) ==-1; }
    
    /*─······································································─*/

    bool operator==( const array_t& oth ) const noexcept { return compare( oth ) == 0; }
    bool operator!=( const array_t& oth ) const noexcept { return compare( oth ) != 0; }

    T& operator[]( ulong n ) const noexcept { return buffer[n]; }
    
    /*─······································································─*/

    long index_of( function_t<bool,T&> func ) const noexcept { long i=0;
        for( auto x : *this ){ if( func(x) ) return i; i++; } return -1;
    }

    ulong count( function_t<bool,T&> func ) const noexcept { ulong n=0; 
        for( auto x : *this ){ if( func(x) ) n++; } return n;
    }
    
    /*─······································································─*/

    T reduce( function_t<T,T&,T&> func ) const noexcept { T act = (*this)[0];
        for( auto x=this->begin() + 1; x != this->end(); x++ )
           { act = func( act, *x ); } return act;
    }

    bool some( function_t<bool,T&> func ) const noexcept { 
        for( auto x : *this ){ if( func(x) ) return 1; } return 0;
    }

    bool none( function_t<bool,T&> func ) const noexcept { 
        for( auto x : *this ){ if( func(x) ) return 0; } return 1;
    }

    bool every( function_t<bool,T&> func ) const noexcept { 
        for( auto x : *this ){ if( func(x) ) return 0; } return 1;
    }

    void map( function_t<void,T&> func ) const noexcept { 
        for( auto x : *this ){ func(x); }
    }

    /*─······································································─*/
#ifndef ARDUINO

    T max() const noexcept { T n = (*this)[0];
        for( auto x=this->begin() + 1; x != this->end(); x++ ){
            n = ::max( n, *x ); 
        }   return n;
    }

    T min() const noexcept { T n = (*this)[0];
        for( auto x=this->begin() + 1; x != this->end(); x++ ){
            n = ::min( n, *x ); 
        }   return n;
    }
    
#endif
    /*─······································································─*/

    int compare( const array_t& oth ) const noexcept {
        if( size() < oth.size() ){ return -1; }
        if( size() > oth.size() ){ return  1; }
        ulong n = size(); while( n-->0 ){
            if( (*this)[n] < oth[n] ){ return -1; }
            if( (*this)[n] > oth[n] ){ return  1; }
        }   return 0;
    }
    
    /*─······································································─*/

    array_t remove( function_t<bool,T&> func ) noexcept {
        ulong n=size(); while( n-->0 ){ if( func((*this)[n]) ) erase(n); } return (*this);
    }

    array_t copy() const noexcept { auto n_buffer = ptr_t<T>(size());
        ulong n=first(); for( auto x : *this ){ n_buffer[n]=x; n++; } return n_buffer;
    }

    array_t reverse() const noexcept { auto n_buffer = ptr_t<T>(size());
        ulong n=last(); for( auto x : *this ){ n_buffer[n]=x; n--; } return n_buffer;
    }

    array_t replace( function_t<bool,T&> func, T targ ) const noexcept {
        for( auto& x : *this ){ if(func(x)) x=targ; } return (*this); 
    }
    
    /*─······································································─*/

    void fill( const char& argc ) const noexcept { buffer.fill(argc); }

    template< class... V >
    void resize( V... args ) noexcept { buffer.resize(args...); }
    
    /*─······································································─*/

    array_t sort( function_t<bool,T,T> func ) const noexcept {
        auto n_buffer = copy();

        while(1){ int nn = 0; for( ulong i=0; i<size(); i++ ){
            int act=i, prv = i-1; if( prv<0 ) continue;
            char _act = n_buffer[act], _prv = n_buffer[prv];
            if( func( _prv, _act ) == 0 ){ continue; } nn++;
            n_buffer[act] = _prv; n_buffer[prv] = _act;
        } if( nn == 0 ) break; }
        
        return n_buffer;
    }
    
    /*─······································································─*/

    void unshift( T value ) noexcept { insert( first(), value ); }

    void push( T value ) noexcept { insert( size(), value ); }

    void shift() noexcept { erase( first() ); }

    void pop() noexcept { erase( size() ); }
    
    /*─······································································─*/

    void clear() noexcept { buffer.reset(); }

    void erase() noexcept { buffer.reset(); }
    
    /*─······································································─*/

    void insert( ulong index, const T& value ) noexcept {
	    index = clamp( index, 0UL, size() );
        if( empty() ){ buffer = ptr_t<T> ( 1 ); buffer[0] = value; } 
        else { index = index>size() ? size() : index; ulong n=size() + 1;
            auto n_buffer = ptr_t<T>(n); for( ulong x=0,i=0; x<n; x++ ){ 
                if( x == index ){ n_buffer[x] = value; }
                else { n_buffer[x] = buffer[i++]; }
            }   buffer = n_buffer;
        }
    }

    void insert( ulong index, ulong N , T* value ) noexcept {
	    index = clamp( index, 0UL, size() );
        if( empty() ){ buffer = ptr_t<T> ( value, N ); } 
        else { index = index>size() ? size() : index; ulong n=size() + N;
            auto n_buffer = ptr_t<T>(n); for( ulong x=0,i=0,p=0; x<n; x++ ){ 
                if( x>=index && x<index+N ){ n_buffer[x] = value[p++]; }
                else { n_buffer[x] = buffer[i++]; }
            }   buffer = n_buffer;
        }
    }

    void insert( ulong index, ulong N , const T& value ) noexcept {
	    index = clamp( index, 0UL, size() );
        if( empty() ){ buffer = ptr_t<T> ( N, value ); } 
        else { index = index>size() ? size() : index; ulong n=size() + N;
            auto n_buffer = ptr_t<T>(n); for( ulong x=0,i=0; x<n; x++ ){ 
                if( x>=index && x<index+N ){ n_buffer[x] = value; }
                else { n_buffer[x] = buffer[i++]; }
            }   buffer = n_buffer;
        }
    }

    void insert( ulong index, array_t value ) noexcept {
	    index = clamp( index, 0UL, size() );
        if( empty() ){ buffer = ptr_t<T> ( value.size() ); 
            for( ulong i=0; i<value.size(); i++ ){ buffer[i] = value[i]; }
        } else { index = index>size() ? size() : index; ulong n=size() + value.size();
            auto n_buffer = ptr_t<T>(n); for( ulong x=0,i=0,p=0; x<n; x++ ){ 
                if( x>=index && x<index+value.size() ){ n_buffer[x] = value[p++]; }
                else { n_buffer[x] = buffer[i++]; }
            }   buffer = n_buffer;
        }
    }

    template< class V, ulong N >
    void insert( ulong index, const V (&value)[N] ) noexcept {
	    index = clamp( index, 0UL, size() );
        if( empty() ){ buffer = ptr_t<T> ( N ); 
            for( ulong i=0; i<N; i++ ){ buffer[i] = value[i]; }
        } else { index = index>size() ? size() : index; ulong n=size() + N;
            auto n_buffer = ptr_t<T>(n); for( ulong x=0,i=0,p=0; x<n; x++ ){ 
                if( x>=index && x<index+N ){ n_buffer[x] = value[p++]; }
                else { n_buffer[x] = buffer[i++]; }
            }   buffer = n_buffer;
        }
    }
    
    /*─······································································─*/

    void erase( ulong index ) noexcept {
	    index = clamp( index, 0UL, last() );
        if( empty() ){ return; } else {
            auto n_buffer = ptr_t<T>( last() );
            for( ulong i=0, j=0; i<size() && !n_buffer.empty(); i++ ){
                if( i != index ){ n_buffer[j] = buffer[i]; j++; }
            }   buffer = n_buffer;
        }
    }

    void erase( ulong start, ulong end  ) noexcept {
	    start = clamp( start, 0UL, last() ); 
	    end   = clamp(   end, 0UL, last() );
        if( empty() || start > end ){ return; } else {
            if( start >= size() || end >= size() ) erase( last() );
            auto n_buffer = ptr_t<T>( size() - ( end - start ) - 1 );
            for( ulong i=0, j=0; i<size() && !n_buffer.empty(); i++ ){
                if( i < start || i > end ){ n_buffer[j] = buffer[i]; j++; }
            }   buffer = n_buffer;
        }
    }
    
    /*─······································································─*/

    string_t join( string_t c=", " ) const noexcept {
        if( empty() ){ return ""; }
        string_t result; for( auto x=begin(); x!=end(); x++ ){
            result += string::to_string(*x) + ((x==end()-1)?"":c);
        }   return result;
    }
    
    /*─······································································─*/

    array_t slice( long start ) const noexcept {
        
        if( empty() ){ return { 0, 0 }; }
        if( start < 0 ){ start = last() + start; }
        if( (ulong)start > last() ){ start = last(); }

        ulong b = clamp( first() + start, 0UL, last() ); 
        ulong z = last() - b + 1;

        auto n_buffer = ptr_t<T>(z);

        for( ulong x=b,y=0; x<=last(); x++ ){ n_buffer[y++] = buffer[x]; }
        return n_buffer;
    }
    
    /*─······································································─*/

    array_t slice( long start, long end ) const noexcept {
        
        if( empty() ){ return { 0, 0 }; }

        if( start < 0 ){ start = last() + start; }     if( end < 0 ){ end = last() + end; }
        if( (ulong)start > last() ){ start = last(); } if( (ulong)end > last() ){ end = last(); }
        if( end < start ){ end = last(); }
        
        if( start > end ){ return { 0, 0 }; }

        ulong a = clamp( first() +   end, 0UL, last() );
        ulong b = clamp( first() + start, 0UL, a ); 
        ulong z = a - b + 1;

        auto n_buffer = ptr_t<T>(z);

        for( ulong x=b,y=0; x<=a; x++ ){ n_buffer[y++] = buffer[x]; }
        return n_buffer;
    }
    
    /*─······································································─*/
 
    array_t splice( long start, ulong del ) noexcept { 
        
        if( empty() || del == 0 ){ return { 0, 0 }; }
        
        if( start < 0 ){ start = last() + start; } if( (ulong)start > last() ){ start = last(); }
            del += start - 1;
        if( del > last() ){ del = last(); } if( del < (ulong)start ){ return { 0, 0 }; }

        ulong a = clamp( first() +   del, 0UL, last() );
        ulong b = clamp( first() + start, 0UL, a ); 
        ulong z = a - b + 1;

        auto n_buffer = ptr_t<T>(z);

        for( ulong x=b,y=0; x<=a; x++ ){ n_buffer[y++] = buffer[x]; }  
        erase( b, a ); return n_buffer;
    }

    template< class V, ulong N >
    array_t splice( long start, ulong del, const V (&value)[N] ) noexcept {
        
        if( empty() || del == 0 ){ return { 0, 0 }; }
        
        if( start < 0 ){ start = last() + start; } if( (ulong)start > last() ){ start = last(); }
            del += start - 1; 
        if( del > last() ){ del = last(); } if( del < (ulong)start ){ return { 0, 0 }; }

        ulong a = clamp( first() +   del, 0UL, last() );
        ulong b = clamp( first() + start, 0UL, a ); 
        ulong z = a - b + 1;

        auto n_buffer = ptr_t<T>(z);

        for( ulong x=b,y=0; x<=a; x++ ){ n_buffer[y++] = buffer[x]; }  
        erase( b, a ); insert( start, value ); return n_buffer;
    }
    
    /*─······································································─*/
    
    explicit operator bool(void) const noexcept { return empty(); }

    explicit operator T*(void) const noexcept { return &buffer; }

    const T* c_arr() const noexcept { return &buffer; }

          T* data() const noexcept { return &buffer; }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace string {

    array_t<string_t> split( string_t _str, char ch ){
        array_t<string_t> result; int c = 0;
        while( (c=_str.index_of([=]( char c ){ return c == ch; })) >= 0 ){
            string_t word = _str.splice( 0, c+1 ); word.pop();
            result.push( word );
        }   result.push( _str ); return result;
    }

    /*─······································································─*/

    array_t<string_t> split( string_t _str, int ch ){
        ch = clamp( (ulong)ch, 1UL, _str.size() );
        array_t<string_t> result; while( !_str.empty() ){
            result.push( _str.splice( 0, ch ) );
        }   return result;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
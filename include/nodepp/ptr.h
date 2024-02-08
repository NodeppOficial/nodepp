#ifndef NODEPP_PTR
#define NODEPP_PTR

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class T > class ptr_t { 
public:

    virtual ~ptr_t() noexcept { reset(); }
    
    /*─······································································─*/

    ptr_t( T* value ) noexcept : length_( new ulong(0) ), count_( new ulong(1) ), value_(value) {}
    ptr_t( const ulong& n, const T& value ) noexcept { resize( n, value ); }
    ptr_t( T* value, const ulong& n ) noexcept { resize( value, n ); }
    ptr_t( const ulong& n ) noexcept { resize( n ); }
    ptr_t() noexcept { resize( nullptr, 0 ); }
    
    /*─······································································─*/

    ptr_t( ptr_t&& other ) noexcept { mve( type::move(other) ); }
    ptr_t( const ptr_t& other ) noexcept { cpy(other); }
    
    /*─······································································─*/

    template < class V, ulong N > 
    ptr_t ( const V (&value)[N] ) noexcept { 
        ulong s = 0; this->resize( N ); 
        for( auto x=begin(); x!=end(); x++ )
           { (*x) = value[s]; s++; } 
    }
    
    /*─······································································─*/

    void reset() noexcept {
        if( count() == 0 ){ return; }
        if( --(*count_) == 0 ) {
            if( size( ) == 0 ) {
                     delete    value_;
            } else { delete [] value_; }
                     delete    count_;
                     delete   length_;
        }
        length_= nullptr;
        count_ = nullptr;
        value_ = nullptr;
    }
    
    /*─······································································─*/

    bool operator> ( const ptr_t& oth ) const noexcept { return this->value_> oth.value_; }
    bool operator>=( const ptr_t& oth ) const noexcept { return this->value_>=oth.value_; }
    bool operator< ( const ptr_t& oth ) const noexcept { return this->value_< oth.value_; }
    bool operator<=( const ptr_t& oth ) const noexcept { return this->value_<=oth.value_; }
    
    /*─······································································─*/

    bool operator==( ptr_t<T> B ) const noexcept { return value_ == B.value_; }
    bool operator!=( ptr_t<T> B ) const noexcept { return value_ != B.value_; }

    bool operator==( T* value ) const noexcept { return value_ == value; }
    bool operator!=( T* value ) const noexcept { return value_ != value; }

    T& operator[]( ulong i ) const noexcept { return value_[i]; }
    
    /*─······································································─*/

    ptr_t& operator=( const ptr_t& other ) noexcept {
        if( this->value_ != &other ){ reset(); cpy(other); }
        return *this;
    }

    ptr_t& operator=( ptr_t&& other ) noexcept {
        if( this->value_ != &other ){ reset(); mve(type::move(other)); }
        return *this;
    }
    
    /*─······································································─*/
    
    void fill( const T& c ) const noexcept {
        if( size() != 0 ) for( auto x=begin(); x!=end(); x++ ) (*x) = c;
        else              *value_ = c;
    }
    
    /*─······································································─*/

    ptr_t copy() const noexcept { auto n_buffer = ptr_t { size() };
        ulong n=0; for( auto x : *this ){ n_buffer[n]=x; n++; } return n_buffer;
    }
    
    /*─······································································─*/

    void resize( ulong n, const T& c ) noexcept { 
        reset(); if( n == 0 ){ return; }
        length_= new ulong( n ); 
        count_ = new ulong( 1 );
        value_ = new T[n];
        while( n-->0 ) value_[n] = c;
    }

    void resize( T* c, ulong n ) noexcept { 
        reset(); if( n == 0 ){ return; }
        length_= new ulong( n ); 
        count_ = new ulong( 1 );
	    value_ = c;
    }
    
    void resize( ulong n ) noexcept { 
        reset(); if( n == 0 ){ return; }
	    if( n <= 0 ) return;
        length_= new ulong( n ); 
        count_ = new ulong( 1 );
	    value_ = new T[n];
    }

    /*─······································································─*/

    ulong  size() const noexcept { return length_ == nullptr ? 0 : *length_; }
    ulong count() const noexcept { return count_  == nullptr ? 0 : *count_; }
    bool  empty() const noexcept { return null() ||( size() <= 0 ); }
    bool   null() const noexcept { return value_  == nullptr; }
    T*     data() const noexcept { return value_; }
    
    /*─······································································─*/

    T*    end() const noexcept { return value_ + size(); }
    void free()       noexcept { *count_ = 1; reset(); }
    T*  begin() const noexcept { return value_; }
    
    /*─······································································─*/

    explicit operator bool(void) const { return null(); }
    explicit operator   T*(void) const { return value_; }

    T* operator->() const noexcept { return  value_; }
    T& operator*()  const noexcept { return *value_; }
    T* operator&()  const noexcept { return  value_; }
    
    /*─······································································─*/

protected:

    ulong* length_= nullptr;
    ulong* count_ = nullptr;
        T* value_ = nullptr;
    
    /*─······································································─*/

    void cpy( const ptr_t& other ) noexcept {
        if( other.count() == 0 ){ return; }
        length_= other.length_;
        count_ = other.count_;
        value_ = other.value_;
        ++(*count_);
    }
    
    /*─······································································─*/

    void mve( ptr_t&& other ) noexcept {
        count_ = other.count_;
        value_ = other.value_;
        length_= other.length_;
        other.count_ = nullptr;
        other.value_ = nullptr;
        other.length_= nullptr;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
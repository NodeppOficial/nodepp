#ifndef NODEPP_STRING
#define NODEPP_STRING

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace string {

    bool   is_hex( uchar c ){ return ( ( c>='0' && c<='9' ) || ( c>='A' && c<='F' ) || ( c>='a' && c<='f' ) ); }
    bool is_space( uchar c ){ return ( c==' ' || c=='\t' || c=='\n' || c=='\r' || c=='\f' || c=='\v' ); }
    bool is_alpha( uchar c ){ return ( ( c>='A' && c<='Z' ) || ( c>='a' && c<='z' ) ); }
    bool is_graph( uchar c ){ return ( c>=33 && c<=126 && c!=' ' ); }
    bool is_lower( uchar c ){ return ( c>='a' && c<='z' ); }
    bool is_upper( uchar c ){ return ( c>='A' && c<='Z' ); }
    bool is_digit( uchar c ){ return ( c>='0' && c<='9' ); }
    bool is_print( uchar c ){ return ( c>=32 && c<=127 ); }
    bool is_contr( uchar c ){ return ( c<32 || c==127 ); }
    bool  is_null( uchar c ){ return ( c=='\0' ); }
    bool is_ascii( uchar c ){ return ( c<=127 ); }
    
    /*─······································································─*/

    ptr_t<char> buffer( ulong n ){ 
        n++; auto b = ptr_t<char>( n, '\0' ); return b; 
    }

    ptr_t<char> buffer( const char* c, ulong n ){
        n++; auto b = ptr_t<char>( n, '\0' );
        while( n-->0 ){ b[n] = c[n]; } return b; 
    }

    ptr_t<char> buffer( ulong n, const char& c ){
        n++; auto b = ptr_t<char>( n, '\0' ); 
        while( n-->0 ){ b[n] = c; } return b; 
    }

    ptr_t<char> null(){ return string::buffer( "", 0 ); }
    
    /*─······································································─*/

    char to_upper( uchar c ){ return is_lower(c) ? ( c - 'a' + 'A' ) : c; }
    char to_lower( uchar c ){ return is_upper(c) ? ( c - 'A' + 'a' ) : c; }
    
    /*─······································································─*/

    bool is_alnum( uchar c ){ return ( is_alpha(c) || is_digit(c) ); }
    bool is_punct( uchar c ){ return ( is_print(c) && !is_space(c) && !is_alnum(c) ); }
    
    /*─······································································─*/

    int char_code( uchar c ){ return (int) c; }

}

/*────────────────────────────────────────────────────────────────────────────*/

class string_t {
protected: 
    
    ptr_t<char> buffer;
    
public:
    
    virtual ~string_t(){  }

    string_t() noexcept { buffer = ""; }

    string_t( const char* argc ) noexcept { ulong n=0;
        if( argc == nullptr || (n=strlen(argc))==0 ){ 
            buffer = ""; return;
        }   buffer = string::buffer( argc, n );
    }

    string_t( const ulong& n, const char& c ) noexcept {
        if( n==0 ){ 
            buffer = ""; return;
        }   buffer = string::buffer( n, c );
    }

    string_t( const char* argc, const ulong& n ) noexcept {
        if( argc == nullptr || n == 0 ){ 
            buffer = ""; return;
        }   buffer = string::buffer( argc, n );
    }
    
    /*─······································································─*/

    string_t( const ptr_t<char>& argc ) noexcept { buffer = argc; }

    /*─······································································─*/

    char* end() const noexcept { return &buffer + size(); }

    char* begin() const noexcept { return &buffer; }
    
    /*─······································································─*/

    ulong first() const noexcept { return 0; }
    bool  empty() const noexcept { return buffer.size() <= 1; }
    ulong  size() const noexcept { return empty() ? 0 : buffer.size() - 1; }
    ulong  last() const noexcept { return empty() ? 0 : buffer.size() - 2; }
    
    /*─······································································─*/

    string_t operator+=( const string_t& oth ){ 
        if( oth.empty() ){ return *this; } string_t ths = copy(); ulong n = 0;
        buffer = string::buffer( this->size() + oth.size() );
        for( auto x : ths ){ (*this)[n] = x; n++; }
        for( auto x : oth ){ (*this)[n] = x; n++; } return *this;
    }
    
    /*─······································································─*/

    bool operator> ( const string_t& oth ) const noexcept { return compare( oth ) == 1; }
    bool operator>=( const string_t& oth ) const noexcept { return compare( oth ) >= 0; }
    bool operator<=( const string_t& oth ) const noexcept { return compare( oth ) <= 0; }
    bool operator< ( const string_t& oth ) const noexcept { return compare( oth ) ==-1; }
    
    /*─······································································─*/

    bool operator==( const string_t& oth ) const noexcept { return compare( oth ) == 0; }
    bool operator!=( const string_t& oth ) const noexcept { return compare( oth ) != 0; }
    
    char& operator[]( ulong n ) const noexcept { return buffer[n]; }
    
    /*─······································································─*/

    long index_of( function_t<bool,char&> func ) const noexcept { long i=0;
        for( auto x : *this ){ if( func(x) ) return i; i++; } return -1;
    }

    ulong count( function_t<bool,char&> func ) const noexcept { ulong n=0; 
        for( auto x : *this ){ if( func(x) ) n++; } return n;
    }
    
    /*─······································································─*/

    char reduce( function_t<char,char&,char&> func ) const noexcept { char act = (*this)[0];
        for( auto x=this->begin() + 1; x != this->end(); x++ )
           { act = func( act, *x ); } return act;
    }

    bool some( function_t<bool,char&> func ) const noexcept { 
        for( auto x : *this ){ if( func(x) ) return 1; } return 0;
    }

    bool none( function_t<bool,char&> func ) const noexcept { 
        for( auto x : *this ){ if( func(x) ) return 0; } return 1;
    }

    bool every( function_t<bool,char&> func ) const noexcept { 
        for( auto x : *this ){ if(!func(x) ) return 0; } return 1;
    }

    void map( function_t<void,char&> func ) const noexcept { 
        for( auto x : *this ) func(x);
    }
    
    /*─······································································─*/

    int compare( const string_t& oth ) const noexcept {
        if( size() < oth.size() ){ return -1; }
        if( size() > oth.size() ){ return  1; } 
        ulong n = size(); while( n-->0 ){
            if( (*this)[n] < oth[n] ){ return -1; }
            if( (*this)[n] > oth[n] ){ return  1; }
        }   return 0;
    }
    
    /*─······································································─*/

    string_t remove( function_t<bool,char&> func ) noexcept {
        ulong n=size(); while( n-->0 ){ if( func((*this)[n]) ) erase(n); } return (*this);
    }

    string_t copy() const noexcept { auto n_buffer = string::buffer(size());
        ulong n=first(); for( auto x : *this ){ n_buffer[n]=x; n++; } return n_buffer;
    }

    string_t reverse() const noexcept { auto n_buffer = copy();
        ulong n=size(); for( auto x : *this ){ n--; n_buffer[n]=x; } return n_buffer;
    }
    
    string_t replace( function_t<bool,char&> func, char targ ) const noexcept {
        for( auto& x : *this ){ if(func(x)) x=targ; } return (*this); 
    }

    /*─······································································─*/

    void fill( const char& argc ) const noexcept { buffer.fill(argc); }

    template< class... T >
    void resize( T... args ) noexcept { buffer.resize(args...); }
    
    /*─······································································─*/

    string_t sort( function_t<bool,char,char> func ) const noexcept {
        auto n_buffer = copy();

        while(1){ ulong nn = 0; for( ulong i=0; i<size(); i++ ){
            long act=i, prv = i-1; if( prv<0 ) continue;
            char _act = n_buffer[act], _prv = n_buffer[prv];
            if( func( _prv, _act ) == 0 ){ continue; } nn++;
            n_buffer[act] = _prv; n_buffer[prv] = _act;
        } if( nn == 0 ) break; }
        
        return n_buffer;
    } 
    
    /*─······································································─*/

    void unshift( char value ) noexcept { insert( first(), value ); }
    void push( char value ) noexcept { insert( size(), value ); }
    void shift() noexcept { erase( first() ); }
    void pop() noexcept { erase( size() ); }
    
    /*─······································································─*/

    void clear() noexcept { buffer.reset(); }
    void erase() noexcept { buffer.reset(); }
    
    /*─······································································─*/

    void insert( ulong index, const char& value ) noexcept {
	    index = clamp( index, 0UL, size() );
        if( empty() ){ buffer = string::buffer( 1 ); buffer[0] = value; } 
        else { index = index > size() ? size() : index; ulong n=size() + 1;
            auto n_buffer = string::buffer(n); for( ulong x=0,i=0; x<n; x++ ){ 
                if( x == index ){ n_buffer[x] = value; }
                else { n_buffer[x] = buffer[i++]; }
            }   buffer = n_buffer;
        }
    }

    void insert( ulong index, ulong N , char* value ) noexcept {
	    index = clamp( index, 0UL, size() );
        if( empty() ){ buffer = string::buffer( value, N ); } 
        else { index = index > size() ? size() : index; ulong n=size() + N;
            auto n_buffer = string::buffer(n); for( ulong x=0,i=0,p=0; x<n; x++ ){ 
                if( x>=index && x<index+N ){ n_buffer[x] = value[p++]; }
                else { n_buffer[x] = buffer[i++]; }
            }   buffer = n_buffer;
        }
    }

    void insert( ulong index, ulong N , const char& value ) noexcept {
	    index = clamp( index, 0UL, size() );
        if( empty() ){ buffer = string::buffer( N, value ); } 
        else { index = index > size() ? size() : index; ulong n=size() + N;
            auto n_buffer = string::buffer(n); for( ulong x=0,i=0; x<n; x++ ){ 
                if( x>=index && x<index+N ){ n_buffer[x] = value; }
                else { n_buffer[x] = buffer[i++]; }
            }   buffer = n_buffer;
        }
    }

    void insert( ulong index, string_t value ) noexcept {
	    index = clamp( index, 0UL, size() );
        if( empty() ){ buffer = string::buffer( value.size() ); 
            for( ulong i=0; i<value.size(); i++ ){ buffer[i] = value[i]; }
        } else { index = index > size() ? size() : index; ulong n=size() + value.size();
            auto n_buffer = string::buffer(n); for( ulong x=0,i=0,p=0; x<n; x++ ){ 
                if( x>=index && x<index+value.size() ){ n_buffer[x] = value[p++]; }
                else { n_buffer[x] = buffer[i++]; }
            }   buffer = n_buffer;
        }
    }
    
    /*─······································································─*/

    void erase( ulong index ) noexcept {
	    index = clamp( index, 0UL, last() );
        if( empty() ){ return; } else {
            index = index >= size() ? last() : index;
            auto n_buffer = string::buffer( size() - 1 );
            for( ulong i=0, j=0; i<size(); i++ ){
                if( i != index ){ n_buffer[j] = buffer[i]; j++; }
            }   buffer = n_buffer;
        }
    }

    void erase( ulong start, ulong end  ) noexcept {
	    start = clamp( start, 0UL, last() ); 
	    end   = clamp(   end, 0UL, last() );
        if( empty() || start > end ){ return; } else {
            if( start >= size() || end >= size() ) erase( last() );
            auto n_buffer = string::buffer( size() - ( end - start ) - 1 );
            for( ulong i=0, j=0; i<size(); i++ ){
                if( i < start || i > end ){ n_buffer[j] = buffer[i]; j++; }
            }   buffer = n_buffer;
        }
    }
    
    /*─······································································─*/
    
    string_t slice( long start ) const noexcept {
        
        if( empty() ){ return ""; }
        if( start < 0 ){ start = last() + start; }
        if( (ulong)start > last() ){ start = last(); }

        ulong b = clamp( first() + start, 0UL, last() ); 
        ulong z = last() - b + 1;

        auto n_buffer = string::buffer(z);

        for( ulong x=b,y=0; x<=last(); x++ ){ n_buffer[y++] = buffer[x]; }
        return n_buffer;
    }
    
    /*─······································································─*/
    
    string_t slice( long start, long end ) const noexcept {
        
        if( empty() ){ return ""; }

        if( start < 0 ){ start = last() + start; }      if( end < 0 ){ end = last() + end; }
        if( (ulong)start > last() ){ start = last(); }  if( (ulong)end > last() ){ end = last(); }
        if( end < start ){ end = last(); }

        if( start > end ){ return ""; }

        ulong a = clamp( first() +   end, 0UL, last() );
        ulong b = clamp( first() + start, 0UL, a ); 
        ulong z = a - b + 1;

        auto n_buffer = string::buffer(z);

        for( ulong x=b,y=0; x<=a; x++ ){ n_buffer[y++] = buffer[x]; }
        return n_buffer;
    }
    
    /*─······································································─*/
 
    string_t splice( long start, ulong del ) noexcept { 
        
        if( empty() || del == 0 ){ return ""; }

        if( start < 0 ){ start = last() + start; } if( (ulong)start > last() ){ start = last(); }
            del += start - 1;
        if( del > last() ){ del = last(); } if( del < (ulong)start ){ return ""; }

        ulong a = clamp( first() +   del, 0UL, last() );
        ulong b = clamp( first() + start, 0UL, a ); 
        ulong z = a - b + 1;

        auto n_buffer = string::buffer(z);

        for( ulong x=b,y=0; x<=a; x++ ){ n_buffer[y++] = buffer[x]; }  
        erase( b, a ); return n_buffer;
    }

    template< class V >
    string_t splice( long start, ulong del, const V& value ) noexcept {
        
        if( empty() || del == 0 ){ return ""; }

        if( start < 0 ){ start = last() + start; } if( (ulong)start > last() ){ start = last(); }
            del += start - 1; 
        if( del > last() ){ del = last(); } if( del < (ulong)start ){ return ""; }

        ulong a = clamp( first() +   del, 0UL, last() );
        ulong b = clamp( first() + start, 0UL, a ); 
        ulong z = a - b + 1;

        auto n_buffer = string::buffer(z);

        for( ulong x=b,y=0; x<=a; x++ ){ n_buffer[y++] = buffer[x]; }  
        erase( b, a ); insert( start, value ); return n_buffer;
    }
    
    /*─······································································─*/

    string_t to_capital_case() const noexcept { if( empty() ){ return (*this); } bool b=1; 
        for( auto x = this->begin(); x!=this->end(); x++ ){
            if( string::is_alpha(*x) && b==1 ){ (*x) = string::to_upper(*x); b=0; continue; }
            if(!string::is_alpha(*x) ){ b=1; }  (*x) = string::to_lower(*x);
        }   return (*this);
    }

    string_t to_lower_case() const noexcept { if( empty() ){ return (*this); }
        for( auto x = this->begin(); x!=this->end(); x++ ){
            (*x) = string::to_lower( (int)(*x) );
        }   return (*this);
    }

    string_t to_upper_case() const noexcept { if( empty() ){ return (*this); }
        for( auto x = this->begin(); x!=this->end(); x++ ){
            (*x) = string::to_upper( (int)(*x) );
        }   return (*this);
    }

    /*─······································································─*/

    explicit operator char* (void) const noexcept { return empty() ? (char*)"" : &buffer; }

          char* data() const noexcept { return empty() ? (char*)"" : &buffer; }

    const char* c_str() const noexcept { return empty() ? "" : &buffer; }

    explicit operator bool(void) const noexcept { return empty(); }
    
};

/*────────────────────────────────────────────────────────────────────────────*/

string_t operator+( const string_t& A, const string_t& B ){
    string_t C = string::buffer( A.size() + B.size() ); ulong n = 0;
    for( auto x : A ){ C[n] = x; n++; }
    for( auto x : B ){ C[n] = x; n++; } return C;
}

/*────────────────────────────────────────────────────────────────────────────*/

namespace string {

    int to_int( string_t buffer ){ 
        int out=0; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%d", &out ); return out;
    }

    bool to_bool( string_t buffer ){ 
        int out=0; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%d", &out ); return out;
    }

    double to_double( string_t buffer ){
        double out=0.0f; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%lf", &out ); return out;
    }

    float to_float( string_t buffer ){
        float out=0.0f; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%f", &out ); return out;
    }

    char to_char( string_t buffer ){ 
        char out=0; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%c", &out ); return out;
    }

    uint to_uint( string_t buffer ){
        uint out=0; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%u", &out ); return out;
    }

    void* to_addr( string_t buffer ){
        void* out=nullptr; if( buffer.empty() ){ return out; }
        sprintf( (char*) buffer, "%p", out ); return out;
    }

    wchar to_wchar( string_t buffer ){
        wchar out=0; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%lc", &out ); return out;
    }

    long to_long( string_t buffer ){
        long out=0; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%ld", &out ); return out;
    }

    llong to_llong( string_t buffer ){
        llong out=0; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%lld", &out ); return out;
    }


    ulong to_ulong( string_t buffer ){
        ulong out=0; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%lu", &out ); return out;
    }

    ullong to_ullong( string_t buffer ){
        ullong out=0; if( buffer.empty() ){ return out; }
        sscanf( (char*) buffer, "%llu", &out ); return out;
    }
    
    /*─······································································─*/

    template< class... T >
    string_t format( string_t str, T... args ){
        char buffer[256] = {0}; sprintf( buffer, (char*)str, args... ); 
        return buffer;
    }

    template< class... T >
    int parse( string_t data, string_t str, T... args ){
        return sscanf( (char*)data, (char*)str, args... ); 
    }
    
    /*─······································································─*/
    
    string_t to_hex( uint num ){
        char buffer[32] = {0}; sprintf( buffer, "%x", num ); return buffer;
    }

    string_t to_bin( uint num ){
        char buffer[32] = {0}; uint n=0; do {
            buffer[n] = num & 1 ? '1' : '0'; num >>= 1; n++;
        }   while(num); return buffer;
    }
    
    /*─······································································─*/
    
    string_t to_string( char* num ){ return num; }

    string_t to_string( string_t num ){ return num; }

    string_t to_string( const char* num ){ return num; }

    string_t to_string( char num ){
        char buffer[32] = {0}; sprintf( buffer, "%c", num ); return buffer;
    }

    string_t to_string( uint num ){
        char buffer[32] = {0}; sprintf( buffer, "%u", num ); return buffer;
    }

    string_t to_string( int num ){
        char buffer[32] = {0}; sprintf( buffer, "%d", num ); return buffer;
    }

    string_t to_string( long num ){
        char buffer[32] = {0}; sprintf( buffer, "%ld", num ); return buffer;
    }

    string_t to_string( wchar num ){
        char buffer[32] = {0}; sprintf( buffer, "%lc", num ); return buffer;
    }

    string_t to_string( ulong num ){
        char buffer[32] = {0}; sprintf( buffer, "%lu", num ); return buffer;
    }

    string_t to_string( llong num ){
        char buffer[32] = {0}; sprintf( buffer, "%lld", num ); return buffer;
    }

    string_t to_string( ullong num ){
        char buffer[32] = {0}; sprintf( buffer, "%llu", num ); return buffer;
    }

    string_t to_string( double num ){
        char buffer[32] = {0}; sprintf( buffer, "%lf", num ); return buffer;
    }

    template< class T > string_t to_string( T* num ){
        char buffer[32] = {0}; sprintf( buffer, "%p", (void*)num ); return buffer;
    }

    string_t to_string( float num ){
        char buffer[32] = {0}; sprintf( buffer, "%lf", (double)num ); return buffer;
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
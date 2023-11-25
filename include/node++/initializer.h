#ifndef NODEPP_INITIALIZER
#define NODEPP_INITIALIZER

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class T > class initializer_t : public ptr_t<T> { public:
    
    initializer_t( const ptr_t<T>& arr ) noexcept : ptr_t<T>( arr ) {}

    initializer_t& operator=( const ptr_t<T>& arr ) noexcept { 
        *this = arr; return *this;
    }

    /*─······································································─*/

    template< class... V >
    initializer_t( const T& head, const V&... tail ) noexcept {
        if( this->empty() ) this->resize( sizeof...(V) + 1 );
        ulong index = 0; iterator::map([&]( const T& item ){
            this->value_[index] = item; index++;
        }, head, tail... );
    }

    /*─······································································─*/

    template< class V, ulong N >
    initializer_t& operator=( const V (&arr) [N] ) noexcept {
        this->resize( N ); for( ulong i=0; i<N; i++ )
            { this->value_[i] = arr[i]; } return (*this);
    }

    template< class V, ulong N >
    initializer_t( const V (&arr) [N] ) noexcept {
        this->resize( N ); for( ulong i=0; i<N; i++ )
            { this->value_[i] = arr[i]; }
    }

    /*─······································································─*/

    template< ulong N >
    initializer_t& operator=( const T (&arr) [N] ) noexcept {
        this->resize( N ); for( ulong i=0; i<N; i++ )
            { this->value_[i] = arr[i]; } return (*this);
    }
    
    template< ulong N >
    initializer_t( const T (&arr) [N] ) noexcept {
        this->resize( N ); for( ulong i=0; i<N; i++ )
            { this->value_[i] = arr[i]; }
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
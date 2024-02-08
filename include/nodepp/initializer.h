#ifndef NODEPP_INITIALIZER
#define NODEPP_INITIALIZER

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class T > class initializer_t : public ptr_t<T> { 
    public: initializer_t() noexcept : ptr_t<T>(){}

    /*─······································································─*/

    template< class V, class = typename type::enable_if<!type::is_same<V,string_t>::value,T>::type >
    initializer_t& operator=( const V& arr ) noexcept {
        this->resize( arr.size() ); for( ulong i=0; i<arr.size(); i++ )
            { this->value_[i] = type::cast<T>( arr[i] ); } return (*this);
    }
    
    template< class V, class = typename type::enable_if<!type::is_same<V,string_t>::value,T>::type >
    initializer_t( const V& arr ) noexcept {
        this->resize( arr.size() ); for( ulong i=0; i<arr.size(); i++ )
            { this->value_[i] = type::cast<T>( arr[i] ); }
    }

    /*─······································································─*/

    initializer_t& operator=( const array_t<T>& arr ) noexcept {
        this->resize( arr.size() ); for( ulong i=0; i<arr.size(); i++ )
            { this->value_[i] = arr[i]; } return (*this);
    }
    
    initializer_t( const array_t<T>& arr ) noexcept {
        this->resize( arr.size() ); for( ulong i=0; i<arr.size(); i++ )
            { this->value_[i] = arr[i]; }
    }

    /*─······································································─*/

    template< class... V >
    initializer_t( const T& head, const V&... tail ) noexcept {
        if( this->empty() ) this->resize( sizeof...(V) + 1 );
        ulong index = 0; iterator::map([&]( const T& item ){
            this->value_[index] = (T)item; index++;
        }, head, tail... );
    }

    /*─······································································─*/

    template< class V, ulong N >
    initializer_t& operator=( const V (&arr) [N] ) noexcept {
        this->resize( N ); for( ulong i=0; i<N; i++ )
            { this->value_[i] = (T)arr[i]; } return (*this);
    }

    template< class V, ulong N >
    initializer_t( const V (&arr) [N] ) noexcept {
        this->resize( N ); for( ulong i=0; i<N; i++ )
            { this->value_[i] = (T)arr[i]; }
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

    /*─······································································─*/
    
    initializer_t( const ptr_t<T>& arr ) noexcept : ptr_t<T>( arr ) {}

    initializer_t& operator=( const ptr_t<T>& arr ) noexcept { 
        *this = arr; return *this;
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
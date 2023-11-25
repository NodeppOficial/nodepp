#ifndef NODEPP_MAP
#define NODEPP_MAP

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template<class U, class V>
class map_t : public array_t<type::pair<U,V>> { public:

    using T = type::pair<U,V>; map_t() noexcept = default;
    
    /*─······································································─*/

    template< class... O >
    map_t( const T& argc, const O&... args ) noexcept {
        this->buffer = ptr_t<T>( sizeof...(args) + 1 );
        ulong x = 0; iterator::map([&]( T arg ){
            this->buffer[x] = arg; x++;
        }, argc, args... );
    }
    
    /*─······································································─*/

    template< ulong N >
    map_t& operator=( const T (&args) [N] ) noexcept {
        this->buffer = ptr_t<T>( N );
        for( ulong x=N; x--; )
            { this->buffer[x] = args[x]; } return *this;
    }

    template< ulong N >
    map_t( const T (&args) [N] ) noexcept { 
        this->buffer = ptr_t<T>( N );
        for( ulong x=N; x--; )
             this->buffer[x] = args[x];
    }
    
    /*─······································································─*/

    V& operator[]( U name ) noexcept {
        
        for( ulong x=0; x<this->size(); x++ ){
            if( this->buffer[x].first == name )
            return this->buffer[x].second;
        }
        
        auto item = (T){name,0}; this->push(item);
        return this->buffer[this->last()].second;
    }
    
    /*─······································································─*/

    void erase( U name ) noexcept {
        if( this->empty() ){ return; } else {
            auto n_buffer = ptr_t<T>( this->last() );
            for( ulong i=0, j=0; i<this->size() && !n_buffer.empty(); i++ ){
                if( this->buffer[i].first != name ){ n_buffer[j] = this->buffer[i]; j++; }
            }   this->buffer = n_buffer;
        }
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
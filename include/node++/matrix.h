#ifndef NODEPP_MATRIX
#define NODEPP_MATRIX

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class T > class matrix_t {
protected:

    ptr_t<T>   buffer;
    ptr_t<int> dimn;

private:

    template< class V >
    void resize_matrix( V* arr, ulong N ){
        ulong size = 1; dimn.resize(N);
        for( auto x=N; x--; ){
             size   *= arr[x];
             dimn[x] = arr[x];
        }    buffer.resize(size);
    }

    void resize_matrix( T* arr, ulong N ){
        ulong size = 1; dimn.resize(N);
        for( auto x=N; x--; ){
             size   *= arr[x];
             dimn[x] = arr[x];
        }    buffer.resize(size);
    }

public: matrix_t() noexcept {} 
    
    /*─······································································─*/

    template< class V, ulong N >
    matrix_t( const V (&arr) [N] ) noexcept {
        resize_matrix( arr, N );
    }

    template< ulong N >
    matrix_t( const T (&arr) [N] ) noexcept {
        resize_matrix( arr, N );
    }
    
    /*─······································································─*/

    template< class V >
    void fill( const V& arg ) const noexcept { buffer.fill( arg ); }

    void fill( const T& arg ) const noexcept { buffer.fill( arg ); }

    void fill( const ptr_t<T>& arg ) const { buffer = arg; }
    
    /*─······································································─*/

    template< class V, ulong N >
    void resize( const V (&arr) [N] ) noexcept {
         resize_matrix( arr, N );
    }

    template< ulong N >
    void resize( const T (&arr) [N] ) noexcept {
         resize_matrix( arr, N );
    }
    
    /*─······································································─*/

    template< ulong N >
    ulong index( const int (&arg) [N] ) const {
        if( N != dimn.size() ) $Error("Wrong Dimension Size");
        ulong size = arg[0]; for( ulong x=1,y=1; x<N; x++ ){
              y *= dimn[x-1]; size+= arg[x] * y;
        }   return size % buffer.size();
    }

    template< class V, ulong N >
    void set( const int (&arg) [N], const V& value ) const {
        if( N != dimn.size() ) $Error("Wrong Dimension Size");
        buffer[ index( arg ) ] = value;
    }

    template< ulong N >
    void set( const int (&arg) [N], const T& value ) const {
        if( N != dimn.size() ) $Error("Wrong Dimension Size");
        buffer[ index( arg ) ] = value;
    }

    template< ulong N >
    T& get( const int (&arg) [N] ) const {
        if( N != dimn.size() ) $Error("Wrong Dimension Size");
        return buffer[ index( arg ) ];
    }
    
    /*─······································································─*/

    ulong    get_buffer_size() const noexcept { return buffer.size(); }

    T*       get_buffer_data() const noexcept { return &buffer; }

    ptr_t<T>&     get_buffer() const noexcept { return buffer; }

    ptr_t<int> get_dimension() const noexcept { return dimn; }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace matrix {



} }

/*────────────────────────────────────────────────────────────────────────────*/

#endif
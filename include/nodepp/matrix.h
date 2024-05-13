/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

#ifndef NODEPP_MATRIX
#define NODEPP_MATRIX

/*────────────────────────────────────────────────────────────────────────────*/

#include "initializer.h"
#include <cmath>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { 
template< class T >	class matrix_t {
protected:		

	struct NODE {
		uint size[2];
		ptr_t<T> data;
	};	ptr_t<NODE> data;

public:

	matrix_t ( const initializer_t<T>& val, uint x, uint y ) noexcept : 
	    data ( new NODE() ){ resize( x, y ); set( val ); }

	matrix_t ( uint x=0, uint y=0 ) noexcept : 
	    data ( new NODE() ){ resize( x, y ); }

	matrix_t () noexcept :
	    data ( new NODE() ){ resize( 0, 0 ); }
    
    /*─······································································─*/

    T* begin() const noexcept { return data->data.begin(); }

    T*   end() const noexcept { return data->data.end(); }
    
    /*─······································································─*/

	void resize( uint x, uint y ) noexcept {
		data->size[0] = x; data->size[1] = y;
		data->data = ptr_t<T>( (ulong)(x*y), T() );
	}
    
    /*─······································································─*/
		
	void append( uint a, uint b, uint c, uint d ) noexcept {
		uint row = max( data->size[0], data->size[0]+c+d );
		uint col = max( data->size[1], data->size[1]+a+b );
		
		ptr_t<T> mtrx ( (ulong)(col*row), T() ); ulong z = 0;
		
		for( uint y=a; y<data->size[1]+a; y++ ){ 
		for( uint x=c; x<data->size[0]+c; x++ ){
			 mtrx[row*y+x] = data->data[z]; z++;
		}}	 
		
		data->size[0] =row; data->size[1] =col; data->data =mtrx;
	}
		
	void remove( uint a, uint b, uint c, uint d ) noexcept {
		uint row = min( data->size[0], data->size[0]-c-d );
		uint col = min( data->size[1], data->size[1]-a-b );
		
		ptr_t<T> mtrx ( (ulong)(col*row), T() ); ulong z = 0; 

		for( uint y=a; y<data->size[1]-b; y++ ){ 
		for( uint x=c; x<data->size[0]-d; x++ ){
			 mtrx[z] = data->data[data->size[0]*y+x]; z++;
		}}	 
		
		data->size[0] =row; data->size[1] =col; data->data =mtrx;
	}
    
    /*─······································································─*/

	long index_of( function_t<bool,T> func ) const noexcept { long i=0;
        for( auto& x : *this ){ if( func(x) ) return i; i++; } return -1;
    }

    ulong count( function_t<bool,T> func ) const noexcept { ulong n=0; 
        for( auto& x : *this ){ if( func(x) ) n++; } return n;
    }
    
    /*─······································································─*/

    T reduce( function_t<T,T,T> func ) const noexcept { T act = (*this)[0];
        for( auto x=this->begin() + 1; x != this->end(); x++ )
           { act = func( act, *x ); } return act;
    }

    bool some( function_t<bool,T> func ) const noexcept { 
        for( auto& x : *this ){ if( func(x) ) return 1; } return 0;
    }

    bool none( function_t<bool,T> func ) const noexcept { 
        for( auto& x : *this ){ if( func(x) ) return 0; } return 1;
    }

    bool every( function_t<bool,T> func ) const noexcept { 
        for( auto& x : *this ){ if( func(x) ) return 0; } return 1;
    }
    
    /*─······································································─*/

	void map( function_t<void,int,int,T&> callback ) const noexcept {
		 for( uint y=0; y<data->size[1]; y++ ){ 
		 for( uint x=0; x<data->size[0]; x++ ){
			  callback( x, y, get(x,y) );
		 }} 
	}

	void map( function_t<void,int,T&> callback ) const noexcept {
		 for( uint x=0; x<data->data.size(); x++ ){ 
			  callback( x, get(x) );
		 }
	}
    
    /*─······································································─*/

    bool operator> ( const matrix_t& oth ) const noexcept { return compare( oth ) == 1; }
    bool operator>=( const matrix_t& oth ) const noexcept { return compare( oth ) >= 0; }
    bool operator<=( const matrix_t& oth ) const noexcept { return compare( oth ) <= 0; }
    bool operator< ( const matrix_t& oth ) const noexcept { return compare( oth ) ==-1; }
    bool operator==( const matrix_t& oth ) const noexcept { return compare( oth ) == 0; }
    bool operator!=( const matrix_t& oth ) const noexcept { return compare( oth ) != 0; }
    
    /*─······································································─*/

    int compare( const matrix_t& oth ) const noexcept {
          if( length() < oth.length() ){ return -1; }
        elif( length() > oth.length() ){ return  1; }
        ulong n = length(); while( n-->0 ){
            if( (*this)[n] < oth[n] ){ return -1; }
            if( (*this)[n] > oth[n] ){ return  1; }
        }   return 0;
    }
    
    /*─······································································─*/

	void set( const initializer_t<T> val ) const noexcept {
		 for( ulong x=val.size(); x--; ){ data->data[x] = val[x]; }
	}

	template< class V, ulong N >
	void set( const V (&val) [N] ) const noexcept {
		 for( ulong x=N; x--; ){ data->data[x] = (T)val[x]; }
	}

	template< ulong N >
	void set( const T (&val) [N] ) const noexcept {
		 for( ulong x=N; x--; ){ data->data[x] = val[x]; }
	}

	void set( const T& val ) const noexcept {
		 for( auto &x : val ){ x = val; }
	}

	void set( uint z, const T& val ) const noexcept {
		 data->data[ z ] = val;
	}

	void set( uint x, uint y, const T& val ) const noexcept {
		 data->data[ data->size[0] * y + x ] = val;
	}
    
    /*─······································································─*/

	T& get( uint x, uint y ) const noexcept {
		return data->data[ data->size[0] * y + x ];
	}

	T& get( uint z ) const noexcept {
		return data->data[ z ];
	}

	ptr_t<T>& get() const noexcept {
		return data->data;
	}
    
    /*─······································································─*/

	template< class... V >
	void operator[]( V... args ) const noexcept {
		 return get( args... );
	}
    
    /*─······································································─*/

	uint*   size() const noexcept { return data->size; }

	ulong length() const noexcept { return data->data.size(); }

	bool   empty() const noexcept { return data->data.empty(); }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

    template< class T >
	matrix_t<T> operator* ( matrix_t<T>& mtrx, T val ){ 
		matrix_t<T> res( mtrx.size()[0], mtrx.size()[1] );
		for ( ulong x=0; x<res.length(); x++ )
			{ res[x] = mtrx[x] * val; }
		return res;
	}

	template< class T >
	matrix_t<T> operator/ ( matrix_t<T>& mtrx, T val ){ 
		matrix_t<T> res( mtrx.size()[0], mtrx.size()[1] );
		for ( ulong x=0; x<res.length(); x++ )
			{ res[x] = mtrx[x] / val; }
		return res;
	}
    
    /*─······································································─*/

	template< class T >
	matrix_t<T> operator+( matrix_t<T>& A, matrix_t<T>& B ){ matrix_t<T> C( B.size()[0], B.size()[1] );
		
		if( A.size()[0] != B.size()[0] || A.size()[1] != B.size()[1] ){
			auto data = string::format( 
				"add (%d,%d) by (%d,%d)", 
				A.size()[0], A.size()[1],
				B.size()[0], B.size()[1] 
			);	process::error( data.c_str() );
		}
			
		for( ulong x = A.length(); x--; ){
			 C.get(x)= A.get(x) + B.get(x);
		}	 return C;
	}

	template< class T >
	matrix_t<T> operator-( matrix_t<T>& A, matrix_t<T>& B ){ matrix_t<T> C( B.size()[0], B.size()[1] );
		
		if( A.size()[0] != B.size()[0] || A.size()[1] != B.size()[1] ){
			auto data = string::format( 
				"sub (%d,%d) by (%d,%d)", 
				A.size()[0], A.size()[1],
				B.size()[0], B.size()[1] 
			);	process::error( data.c_str() );
		}
			
		for( ulong x = A.length(); x--; ){
			 C.get(x)= A.get(x) - B.get(x);
		}	 return C;
	}
    
    /*─······································································─*/

	template< class T >
	matrix_t<T> operator^( matrix_t<T>& A, matrix_t<T>& B ){ matrix_t<T> C( B.size()[0], B.size()[1] );
		
		if( A.size()[0] != B.size()[0] || A.size()[1] != B.size()[1] ){
			auto data = string::format( 
				"mult (%d,%d) by (%d,%d)", 
				A.size()[0], A.size()[1],
				B.size()[0], B.size()[1] 
			);	process::error( data.c_str() );
		}
			
		for( ulong x = A.length(); x--; ){
			 C.get(x)= A.get(x) * B.get(x);
		}	 return C;
	}
    
    /*─······································································─*/

	template< class T >
	matrix_t<T> operator*( matrix_t<T>& A, matrix_t<T>& B ){ matrix_t<T> C( B.size()[0], A.size()[1] );
		
		if( A.size()[0] != B.size()[1] ){ 
			auto data = string::format( 
				"mult (%d,%d) by (%d,%d)", 
				A.size()[0], A.size()[1],
				B.size()[0], B.size()[1] 
			);	process::error( data.c_str() ); 
		}
	
		for( uint y1=0; y1<A.size()[1]; y1++ ){ for( uint x2=0; x2<B.size()[0]; x2++){ 
		for( uint y2=0; y2<B.size()[1]; y2++ ){ for( uint x1=0; x1<A.size()[0]; x1++){ 
		if ( x1 != y2 ){ continue; }
			 C.get( x2, y1 ) += A.get( x1, y1 ) * B.get( x2, y2 );
		}}}}
	
		return C;
	}
    
    /*─······································································─*/

	// operator/

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace matrix {

    /*─······································································─*/

	template< class T >
	matrix_t<T> transpose( matrix_t<T>& mtrx ){
		matrix_t<T> res( mtrx.size()[1], mtrx.size()[0] ); ulong z = 0;
		for( uint y=0; y<res.size()[1]; y++ ){ for( uint x=0; x<res.size()[0]; x++){
			 res.get(z) = mtrx.get( y, x ); z++;
		}}	 return res;
	}
    
    /*─······································································─*/

	template< class T >
	matrix_t<T> flip( matrix_t<T>& mtrx, bool X, bool Y ){
		matrix_t<T> res ( mtrx.size()[0], mtrx.size()[1] ); ulong z=0;
		for( uint y=0; y<res.size()[1]; y++ ){ for( uint x=0; x<res.size()[0]; x++ ){ 
			 uint a = X==0 ? x : res.size()[0]-x-1; 
			 uint b = Y==0 ? y : res.size()[0]-y-1;
			 res.get(z) = mtrx.get( a, b ); z++;
		}}	 return res;
	}
    
    /*─······································································─*/

	template< class T >
	matrix_t<T> min( matrix_t<T>& mtrx, T _min ){
		matrix_t<T> res( mtrx.size()[0], mtrx.size()[1] );
		for ( ulong x=0; x<res.length(); x++ )
			{ res[x] = min( mtrx[x], _min ); }
		return res;
	}

	template< class T >
	matrix_t<T> max( matrix_t<T>& mtrx, T _max ){
		matrix_t<T> res( mtrx.size()[0], mtrx.size()[1] );
		for ( ulong x=0; x<res.length(); x++ )
			{ res[x] = max( mtrx[x], _max ); }
		return res;
	}

	template< class T >
	matrix_t<T> clamp( matrix_t<T>& mtrx, T _min, T _max ){
		matrix_t<T> res( mtrx.size()[0], mtrx.size()[1] );
		for ( ulong x=0; x<res.length(); x++ )
			{ res[x] = clamp( mtrx[x], _min, _max ); }
		return res;
	}
    
    /*─······································································─*/

	template< class T >
	matrix_t<T> abs( matrix_t<T>& mtrx ){
		matrix_t<T> res( mtrx.size()[0], mtrx.size()[1] );
		for ( ulong x=0; x<res.length(); x++ )
			{ res[x] = abs( mtrx[x] ); }
		return res;
	}

	template< class T >
	matrix_t<T> sqrt( matrix_t<T>& mtrx ){
		matrix_t<T> res( mtrx.size()[0], mtrx.size()[1] );
		for ( ulong x=0; x<res.length(); x++ )
			{ res[x] = sqrt( mtrx[x] ); }
		return res;
	}

	template< class T >
	matrix_t<T> pow( matrix_t<T>& mtrx, T val ){
		matrix_t<T> res( mtrx.size()[0], mtrx.size()[1] );
		for ( ulong x=0; x<res.length(); x++ )
			{ res[x] = pow( mtrx[x], val ); }
		return res;
	}

    /*─······································································─*/

	template< class T >
	matrix_t<T> conv( matrix_t<T>& A, matrix_t<T>& B ){ 
	matrix_t<T> C ( A.size()[0]-B.size()[0]+1, A.size()[1]-B.size()[1]+1 );

		if( A.length() < B.length() ){
			auto data = string::format( 
				"conv (%d,%d) by (%d,%d)", 
				A.size()[0], A.size()[1],
				B.size()[0], B.size()[1] 
			);	process::error( data.c_str() ); 
		}

		auto D = matrix::flip( B, 1, 1 );

		for( uint i=C.size()[1]; i--; ){ for( uint j=C.size()[0]; j--; ){ 
		for( uint k=B.size()[1]; k--; ){ for( uint l=B.size()[0]; l--; ){
			 C.get( i, j ) += A.get( i+k, j+l ) * D.get( k, l );
		}}}}

		return C; 
	}

	template< class T >
	matrix_t<T> corr( matrix_t<T>& A, matrix_t<T>& B ){
	matrix_t<T> C ( A.size()[0]-B.size()[0]+1, A.size()[1]-B.size()[1]+1 );

		if( A.length() < B.length() ){
			auto data = string::format( 
				"corr (%d,%d) by (%d,%d)", 
				A.size()[0], A.size()[1],
				B.size()[0], B.size()[1] 
			);	process::error( data.c_str() ); 
		}

		for( uint i=C.size()[1]; i--; ){ for( uint j=C.size()[0]; j--; ){ 
		for( uint k=B.size()[1]; k--; ){ for( uint l=B.size()[0]; l--; ){
			 C.get( i, j ) += A.get( i+k, j+l ) * B.get( k, l );
		}}}}

		return C; 
	}

    /*─······································································─*/

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_ENCODER
#define NODEPP_ENCODER

/*────────────────────────────────────────────────────────────────────────────*/

#include "string.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace encoder {

    template< ulong N >
    string_t hash( const char (&alph) [N], int x=32 ) {
        string_t data; while( x --> 0 ){
            data.push( alph[ rand() % ( N - 1 ) ] );
        }   return data;
    }
    
    /*─······································································─*/

    ulong hash() { int x = sizeof(ulong) * 8; 
            ulong  data = 0; while( x --> 0 ){
            data = data << 1 | ( data | rand() % 2 );
        }   return data;
    }
    
    /*─······································································─*/

    ulong hash( const string_t& key, int tableSize ) {
        ulong hash = 5381; for ( auto c : key ) {
            hash = ((hash << 5) + hash) + c;
        }   return hash % tableSize;
    }
    
    /*─······································································─*/

    ulong hash( int key, int tableSize ) {
        return key % tableSize;
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace encoder { namespace bin {

    template< class T >
    ptr_t<bool> get( T num ){
        ptr_t<bool> res ( sizeof(num) * 8, 0 );
        for ( auto x =sizeof(num)*8; x--; ){
              res[x] = num & 1 ; num >>= 1;
        }     return res;
    }

    template< class T >
    T set( const ptr_t<bool>& num ){ T res = 0;
         if ( num.empty() ){ return res; }
         if ( sizeof(res)*8 < num.size() )
            { process::error("Binary Does Not Fit In"); }
        for ( auto x : num ){
              res = res << 1 | x & 1 ;
        }     return res;
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace encoder { namespace hex {

    template< class T >
    string_t get( T num ){ char bff [ 32 ];
        auto x = sprintf( bff, "%x", num );
        return { bff, (ulong) x };
    }

    template< class T >
    T set( const string_t& num ){ T res = 0;
        if ( num.empty() ){ return res; }
        auto x = fscanf( num.data(), "%x", &res );
        return res;
    }
    
    string_t buff2hex( const ptr_t<uchar>& inp ){
        if ( inp.empty() ){ return nullptr; }
        string_t out; for( auto x : inp ){
            out += string::format( "%02x", x );
        }   return out;
    }

    ptr_t<uchar> hex2buff( string_t x ){
        if ( x.empty() ){ return nullptr; }
        ulong size = x.size() / 2 + ( x.size()%2 ? 0 : 1 ); 
        ptr_t<uchar> out ( size ); for ( auto& y : out ){
            string::parse( x.splice(0,2), "%02x", &y );
        }   return out;
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace encoder { namespace utf8 {

    ptr_t<char16_t> to_utf16( string_t inp ){ 
        if( inp.empty() ){ return nullptr; }
        ptr_t<char16_t> res ( inp.size(), 0 ); 
        for ( ulong x=0; x<inp.size(); x++ ){ 
            res[x] = type::cast<char16_t>( inp[x] );
        }   return res;
    }

    ptr_t<char32_t> to_utf32( string_t inp ){
        if( inp.empty() ){ return nullptr; }
        ptr_t<char32_t> res ( inp.size(), 0 ); 
        for ( ulong x=0; x<inp.size(); x++ ){ 
            res[x] = type::cast<char16_t>( inp[x] );
        }   return res;
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace encoder { namespace utf16 {

    string_t to_utf8( ptr_t<char16_t> inp ){ 
        if ( inp.empty() ){ return nullptr; } string_t res;
        for( ulong x=0; inp[x] != 0; ++x ){ char16_t ch = inp[x];
        if ( ch <= 0x7F ) {
            res.push(type::cast<char>(ch));
        } elif ( ch <= 0x7FF ) {
            res.push(type::cast<char>(0xC0 | ( ch >> 6)));
            res.push(type::cast<char>(0x80 | ( ch & 0x3F)));
        } else {
            res.push(type::cast<char>(0xE0 | ( ch >> 12)));
            res.push(type::cast<char>(0x80 | ((ch >> 6) & 0x3F)));
            res.push(type::cast<char>(0x80 | ( ch & 0x3F)));
        }
        }   return res;
    }

    ptr_t<char32_t> to_utf32( ptr_t<char16_t> inp ){
        if( inp.empty() ){ return nullptr; }
        ptr_t<char32_t> res ( inp.size(), 0 ); 
        for ( ulong x=0; x<inp.size(); x++ ){ 
            res[x] = type::cast<char32_t>( inp[x] );
        }   return res;
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace encoder { namespace utf32 {

    string_t to_utf8( ptr_t<char32_t> inp ){ 
        if ( inp.empty() ){ return nullptr; } string_t res;
        for( ulong x=0; inp[x] != 0; ++x ){ char32_t ch = inp[x];
        if ( ch <= 0x7F ) {
            res.push(type::cast<char>(ch));
        } elif ( ch <= 0x7FF ) {
            res.push(type::cast<char>(( ch >> 6) | 0xC0));
            res.push(type::cast<char>(( ch & 0x3F) | 0x80));
        } elif( ch <= 0xFFFF ) {
            res.push(type::cast<char>(( ch >> 12) | 0xE0));
            res.push(type::cast<char>(((ch >> 6) & 0x3F) | 0x80));
            res.push(type::cast<char>(((ch & 0x3F) | 0x80)));
        } else {
            res.push(type::cast<char>(( ch >> 18) | 0xF0));
            res.push(type::cast<char>(((ch >> 12) & 0x3F) | 0x80));
            res.push(type::cast<char>(((ch >>  6) & 0x3F) | 0x80));
            res.push(type::cast<char>(( ch & 0x3F) | 0x80));       
        }
        }   return res;
    }

    ptr_t<char16_t> to_utf16( ptr_t<char32_t> inp ){
        ptr_t<char16_t> res = ptr_t<char16_t>( (inp.size()+1) * sizeof(char16_t), 0 );

        ulong x = 0; while( inp[x] != 0 ) {
            if( inp[x] < 0x10000 ) {
                res[x] = type::cast<char16_t>( inp[x] );
            } else {
                inp[x]-= 0x10000;
                res[x] = type::cast<char16_t>(0xD800 + (inp[x] >> 10)); x++;
                res[x] = type::cast<char16_t>(0xDC00 + (inp[x] & 0x3FF));
            }   x++;
        }

        res[x] = 0; return res;
    }

}}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
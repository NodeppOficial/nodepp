#ifndef NODEPP_HASH
#define NODEPP_HASH

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace hash {

    template< ulong N >
    string_t hash( const char (&alph) [N] ) {
        int x = 32; string_t data; while( x --> 0 ){
            data.push( alph[ rand() % ( N - 1 ) ] );
        }   return data;
    }
    
    /*─······································································─*/

    ulong hash() {
        int x = 32; ulong data = 0; while( x --> 0 ){
            data = ( data | rand() % 2 ) << 1;
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

#endif
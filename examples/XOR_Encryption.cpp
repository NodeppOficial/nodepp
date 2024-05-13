#define CLAVE "ClavePrivada"

#include <nodepp/nodepp.h>
#include <nodepp/fs.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

void encrypt( const string_t& pathA, const string_t& pathB ){

     string_t key = CLAVE;
     file_t file( pathA, "r" );
     file_t fout( pathB, "w" );

     file.onData([=]( string_t data ){
          string_t output ( data.size(), 0 );
          for( auto x=data.size(); x--; ){
               output[x] = data[x] ^ key[ x % ( key.size() - 1 ) ];
          }    fout.write( output );
     });

     file.onClose([](){
          console::done(" File Encrypted ");
     });

     stream::pipe( file );

}

/*────────────────────────────────────────────────────────────────────────────*/

void decrypt( const string_t& path ){

     string_t key = CLAVE;
     file_t file( path, "r" );

     file.onData([=]( string_t data ){
          string_t output ( data.size(), 0 );
          for( auto x=data.size(); x--; ){
               output[x] = data[x] ^ key[ x % ( key.size() - 1 ) ];
          }    console::log( output );
     });

     file.onClose([](){
          console::done(" File Decrypted ");
     });

     stream::pipe( file );

}

/*────────────────────────────────────────────────────────────────────────────*/

void onMain(){
     
     if ( process::env::get("mode") == "A" )
          encrypt( "LICENSE", "ENC" );
     else decrypt( "ENC" );

}

/*────────────────────────────────────────────────────────────────────────────*/
#include <nodepp/nodepp.h>
#include <nodepp/crypto.h>
#include <nodepp/fs.h>

using namespace nodepp;

void encrypt() {

     auto encr = crypto::enc::AES_128_ECB( "CLAVE" );
     auto fint = fs::readable( "/PATH/TO/FILE/A" );
     auto fout = fs::writable( "/PATH/TO/FILE/B" );

     fint.onData([=]( string_t data ){ encr.update( data ); });

     encr.onData([=]( string_t data ){ fout.write( data ); });

     fint.onDrain([=](){ encr.close(); });

     stream::pipe( fint );

}

void decrypt(){

     auto encr = crypto::dec::AES_256_CBC( "1234567890" );
     auto fint = fs::readable( "/PATH/TO/FILE/B" );
     auto fout = fs::writable( "/PATH/TO/FILE/C" );

     fint.onData([=]( string_t data ){ encr.update( data ); });

     encr.onData([=]( string_t data ){ fout.write( data ); });

     fint.onDrain([=](){ encr.close(); });

     stream::pipe( fint );

}

void _main_() {

     if ( process::env::get("mode") == "A" )
          encrypt();
     else decrypt();

}
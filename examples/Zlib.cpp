#include <nodepp/nodepp.h>
#include <nodepp/zlib.h>
#include <nodepp/fs.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    auto B = fs::writable("mojon.txt");
    auto A = fs::readable("LICENSE");

    A.onClose([](){
        console::log("done");
    });

    zlib::gzip::pipe( A, B );

}

/*────────────────────────────────────────────────────────────────────────────*/
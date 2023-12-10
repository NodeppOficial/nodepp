#include <node++/node++.h>
#include <node++/zlib.h>
#include <node++/fs.h>

using namespace nodepp;

void $Ready() {

    auto B = fs::writable("mojon.txt");
    auto A = fs::readable("LICENSE");

    A.onClose([](){
        console::log("done");
    });

    zlib::gzip::pipe( A, B );

}
#include <node++/node++.h>
#include <node++/zlib.h>
#include <node++/fs.h>

using namespace nodepp;

void _Ready() {

    auto B = fs::writable("mojon.txt");
    auto A = fs::readable("ppt.txt");

    A.onDone([](){
        console::log("done");
    });

    zlib::inflate_pipe( A, B );

}
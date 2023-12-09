#include <node++/node++.h>
#include <node++/stream.h>
#include <node++/exec.h>

using namespace nodepp;

void $Ready() { 
    
    auto x = exec::sync( "curl 'http://www.google.com' -Ls" );
    console::log( x );

}
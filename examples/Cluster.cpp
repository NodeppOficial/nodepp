#include <node++/node++.h>
#include <node++/cluster.h>

using namespace nodepp;

void $Ready() { 

    auto x = cluster::add();

    if( cluster::is_child() ){
        console::log("child");
    } else {
        console::log("parent");
    }

}
#include <nodepp/nodepp.h>
#include <nodepp/cluster.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    auto x = cluster::add();

    if( cluster::is_child() ){
        console::log("child");
    } else {
        console::log("parent");
    }

}

/*────────────────────────────────────────────────────────────────────────────*/
#include <nodepp/nodepp.h>

using namespace nodepp;

onMain([](){

    console::log("message");
    
    console::info("message");
    
    console::done("message");
    
    console::error("message");
    
    console::success("message");

    console::warning("message");

})
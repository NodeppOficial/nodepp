#include <nodepp/nodepp.h>
#include <nodepp/object.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    object_t ppt ({
        { "var1", 1000 },
        { "var2", true },
        { "var3", {{ { "var1", "Hello World!" } }} },
        { "var4", array_t<int>({ 10, 20, 30, 40, 50 }) }
    });

    console::log( (int)  ppt["var1"] );
    console::log( (bool) ppt["var2"] ? "true" : "false" );

    console::log( (string_t)     ppt["var3"]["var1"] );
    console::log(((array_t<int>) ppt["var4"]).join() );

}

/*────────────────────────────────────────────────────────────────────────────*/
#include <node++/node++.h>
#include <node++/object.h>

using namespace nodepp;

void _Ready() { 

    object_t ppt ({
        { "var1", 10 },
        { "var2", 20 },
        { "var3", 30 },
        { "var4", {{
            { "var3", {{
                { "var1", "hola mundo" }
            }} }
        }} }
    });

    ppt["var4"]["var3"]["var1"] = "hola mundo de mierda";

    console::log( (string_t) ppt["var4"]["var3"]["var1"] );

}
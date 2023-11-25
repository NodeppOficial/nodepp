#include <node++/node++.h>
#include <node++/observer.h>

using namespace nodepp;

observer_t obj ({
    { "string", "hola mundo" },
    { "int", 10 }
});

void _Ready() {

    obj.on( "string", []( any_t A, any_t B ){
        console::log( (string_t) A, "|", (string_t) B );
    });

    obj.on( "int", []( any_t A, any_t B ){
        console::log( (int) A, "|", (int) B );
    });

    obj.set([]( observer_t state ){ return observer_t ({
        { "string", (string_t) state["string"] + " de mierda" },
        { "int", (int) state["int"] + 50 }
    }); });

}
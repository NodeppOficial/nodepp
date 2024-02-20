#include <nodepp/nodepp.h>
#include <nodepp/observer.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

observer_t obj ({
    { "string", "Hello Word!" },
    { "int", 10 }
});

/*────────────────────────────────────────────────────────────────────────────*/

onMain([](){

    obj.on( "string", []( any_t A, any_t B ){
        console::log( (string_t) A, "|", (string_t) B );
    });

    obj.on( "int", []( any_t A, any_t B ){
        console::log( (int) A, "|", (int) B );
    });

    obj.set([]( observer_t state ){ return observer_t ({
        { "string", (string_t) state["string"] + "Something" },
        { "int", (int) state["int"] + 50 }
    }); });

})

/*────────────────────────────────────────────────────────────────────────────*/
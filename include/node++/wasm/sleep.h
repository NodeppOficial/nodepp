#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <emscripten.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {
    
    ulong millis(){ return emscripten_get_now(); }

    ulong seconds(){ return emscripten_get_now() / 1000; }

    ulong micros(){ return emscripten_get_now_in_microseconds(); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    void  delay( ulong time ){ emscripten_sleep( time * 1000 ); }

    ulong now(){ return millis(); }

    void yield(){ delay(0); }

}}

/*────────────────────────────────────────────────────────────────────────────*/
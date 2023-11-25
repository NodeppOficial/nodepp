#include <node++/node++.h>

using namespace nodepp;

_Generator( process_1 ) {
    
    ptr_t<int> counter = new int(10);

    _Emit(){ 
    _Start;
        while( (*counter)-->0 ){
            console::done(" :>",*counter); _Again();
        }
    _Stop;
    }

};

void _Ready() {
    process_1 A; process::loop::add(A);
}

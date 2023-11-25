#include <node++/node++.h>
#include <node++/timer.h>
#include <node++/promise.h>

using namespace nodepp;

void _Ready() {

    promise_t<int,int>([=]( auto res, auto rej ){
        timer::delay(1000); rej(10);
    })
    
    .then([=]( int res ){
        console::done(res);
    })
    
    .fail([=]( int rej ){
        console::error(rej);
    });

}
#include <nodepp/nodepp.h>
#include <nodepp/promise.h>
#include <nodepp/timer.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

onMain([](){

    auto t = timer::add([](){
        static int i=0; i++;
        console::log("new interval",i);
        return ( i >= 20 ) ? -1 : 1 ;
    }, 1000 );

    promise::resolve<int>([=]( function_t<void,int> res ){
        timer::timeout([=](){ res(10); },1000);
    },[=]( int res ){
        console::log("first promise resolved:",res);
    });

    promise::resolve<int>([=]( function_t<void,int> res ){
        timer::timeout([=](){ res(20); },5000);
    },[]( int res ){
        console::log("second promise resolved:",res);
    });

    promise::resolve<int>([=]( function_t<void,int> res ){
        timer::timeout([=](){ res(30); },10000);
    },[]( int res ){
        console::log("third promise resolved:",res);
    });

})

/*────────────────────────────────────────────────────────────────────────────*/
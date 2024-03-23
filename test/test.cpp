#include <nodepp/nodepp.h>
#include <nodepp/test.h>

using namespace nodepp;

#include "coroutine.cpp"
#include "console.cpp"
#include "timer.cpp"
#include "regex.cpp"

onMain([](){

    TEST::CONSOLE::TEST_RUNNER();

    console::wait(); 
    console::clear();

    TEST::TIMER::TEST_RUNNER();

    console::wait(); 
    console::clear();

    TEST::COROUTINE::TEST_RUNNER();

    console::wait(); 
    console::clear();

    TEST::REGEX::TEST_RUNNER();

    console::wait(); 
    console::clear();

})
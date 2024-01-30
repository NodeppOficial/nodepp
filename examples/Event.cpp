#include <nodepp/nodepp.h>
#include <nodepp/event.h>
#include <nodepp/timer.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

event_t<> ev; 

void _main_() {

  ev.on([](){ console::log("hello world"); });

  timer::timeout([=](){ ev.emit(); },1000);

  console::log("before event");

}

/*────────────────────────────────────────────────────────────────────────────*/

#include <node++/node++.h>
#include <node++/event.h>
#include <node++/timer.h>

using namespace nodepp;

event_t<> ev; 

void _Ready() {

  ev.on([](){ console::log("hello world"); });

  timer::timeout([=](){ ev.emit(); },1000);

  console::log("before event");

}

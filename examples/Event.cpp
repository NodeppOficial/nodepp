#include <nodepp/nodepp.h>
#include <nodepp/event.h>
#include <nodepp/timer.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

event_t<> ev; 

void _main_() {

  ev.once([](){ console::done(" Bye World! "); });

  ev.on([](){ console::error("hello world 1"); });

  ev.on([](){ console::error("hello world 2"); });

  ev.on([](){ console::error("hello world 3"); });

  ev.on([](){ console::error("hello world 4"); });

  ev.on([](){ console::error("----"); });

  auto x = ev.on([](){ 
    static int x = 0; x++;
    console::info("hello world!", x ); 
  });

  timer::interval([=](){ ev.emit(); }, 1000 );

  timer::timeout([=](){ ev.off( x ); }, 10000 );

  console::log(" before event emitter ");

}

/*────────────────────────────────────────────────────────────────────────────*/

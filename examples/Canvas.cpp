#include <nodepp/nodepp.h>
#include <nodepp/canvas.h>
#include <nodepp/timer.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    display_t dpy;
              dpy.w = 300;
              dpy.h = 300;
              dpy.name = "Window";

    auto ppt = canvas::add( dpy );
    ptr_t<int> x = new int(0);

    timer::add([=](){
        ppt.draw_rectangle( 0xffffff, 0, 0, 500, 500, 0 );
        ppt.draw_rectangle( 0x000000, 5, 5, 285, 285, 1 );
        ppt.draw_string( 0x000000, 10, 30, "Hello World!" );
        ppt.draw_string( 0x000000, 10, 60, string::to_string(*x) );
        ppt.flush(); return 1;
    },1000);

    timer::interval([=](){ 
        console::log("Hello World!");
        *x += 1; 
    },1000);

    ppt.onExit([=](){ process::exit(1); });
}

/*────────────────────────────────────────────────────────────────────────────*/
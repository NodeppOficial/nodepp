#include <nodepp/nodepp.h>
#include <nodepp/canvas.h>
#include <nodepp/timer.h>
#include <nodepp/gl.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    display_t dpy;
              dpy.w = 300;
              dpy.h = 300;
              dpy.name = "Window";

    auto ppt = canvas::add( dpy );
    auto ogl = GL::add( ppt );

    ppt.onDraw([=](){

        glViewport( 0, 0, ppt.obj->info.w, ppt.obj->info.h );
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(-0.5, -0.5);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(0.5, -0.5);
        glColor3f(0.0, 0.0, 1.0);
        glVertex2f(0.0, 0.5);
        glEnd(); 

        ogl.flush();
        
    });
   
    ppt.onExit([=](){ process::exit(1); });
}

/*────────────────────────────────────────────────────────────────────────────*/
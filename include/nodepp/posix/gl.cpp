#pragma once

#include <GL/gl.h>
#include <GL/glx.h>

namespace nodepp { class gl_t {
protected:

    struct _str_ {
        int          state = 0;
        XVisualInfo* info;
        GLXContext   ctx;
        canvas_t     cnv;
    };
    
public: ptr_t<_str_> obj;

    
    /*─······································································─*/

    virtual ~gl_t() noexcept { if( obj.count() > 1 ){ return; } force_close(); }
    
    gl_t( const canvas_t& canvas ) noexcept : obj( new _str_() ){
        if( obj->state == 1 ){ return; } canvas.pipe(); obj->cnv = canvas; obj->state = 1;

        int attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

        obj->info  = glXChooseVisual( obj->cnv.obj->dpy, obj->cnv.obj->id, attr );
        obj->ctx   = glXCreateContext( obj->cnv.obj->dpy, obj->info, NULL, True );
        
        glXMakeCurrent( obj->cnv.obj->dpy, obj->cnv.obj->win, obj->ctx );
    };

    /*─······································································─*/

    void close() const noexcept { obj->state = -1; }

    virtual void force_close() const noexcept { 
        if( obj.count() > 0 && obj->cnv.is_closed() ){ return; }
        glXMakeCurrent( obj->cnv.obj->dpy, None, NULL ); 
        glXDestroyContext( obj->cnv.obj->dpy,obj->ctx );
        obj->state = -1;
    }

    bool is_closed() const noexcept { return obj == nullptr ? 0 : obj->state < 1; }

    /*─······································································─*/

    void flush() const noexcept {
        glXSwapBuffers( obj->cnv.obj->dpy, obj->cnv.obj->win );
        obj->cnv.flush();
    }

};}

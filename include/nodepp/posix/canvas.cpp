#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { struct display_t {
    string_t clss = "MyWindowClass";
    string_t name = "MyWindowName";
    ulong    w = 800, h = 600;
    int      x = 0, y = 0;
    bool     resize =true;
};}

namespace nodepp { class canvas_t {
protected:

    struct _str_ {

        array_t<uint> button, key;
        int mouse[2] = { 0, 0 };

        display_t info;
        int state = 0;

        Window   root;
        Display* dpy;
        Window   win;
        int      id;

        XEvent   event;
        Screen*  scr;
        GC       ctx;

    };

public:
    ptr_t<_str_> obj;

    /*─······································································─*/

    event_t<>          onButtonRelease;
    event_t<>          onButtonPress;
    event_t<>          onMouseMotion;
    event_t<>          onKeyRelease;
    event_t<>          onKeyPress;
    
    event_t<>          onVisibility;
    event_t<>          onFocusOut;
    event_t<>          onFocusIn;
    event_t<>          onResize;
    event_t<>          onEnter;
    event_t<>          onLeave;
    event_t<>          onMove;

    event_t<except_t>  onError;
    event_t<>          onClose;
    event_t<>          onLoop;
    event_t<>          onExit;
    event_t<>          onDraw;
    
    /*─······································································─*/

    canvas_t() noexcept : obj( new _str_() ){}

    virtual ~canvas_t() noexcept { if( obj.count() > 1 ){ return; } force_close(); }

    canvas_t( const display_t& _str ) noexcept : obj( new _str_() ){ obj->info = _str; }
    
    /*─······································································─*/

    ptr_t<int> get_mouse_position() const noexcept { int rootX, rootY, winX, winY; uint mask; Window child;
        XQueryPointer( obj->dpy, obj->root, &obj->root, &child, &rootX, &rootY, &winX, &winY, &mask);
        return {{ winX, winY, rootX, rootY }};
    }

    ptr_t<ulong> get_screen_size() const noexcept { return {{ obj->scr->width, obj->scr->height }}; }

    ptr_t<int> get_screen_position() const noexcept { XWindowAttributes attr;
        XGetWindowAttributes( obj->dpy, obj->root, &attr);
        return {{ attr.x, attr.y }}; 
    }

    ptr_t<int> get_canvas_size() const noexcept { XWindowAttributes attr;
        XGetWindowAttributes( obj->dpy, obj->root, &attr);
        return {{ attr.width, attr.height }}; 
    }
    
    /*─······································································─*/

    bool is_button_released( uint btn ) const noexcept {
        if( obj->button.empty() ) return 1; 
        else { for( auto x : obj->button ){
            if( x == btn ) return 0;
        }}  return 1;
    }

    bool is_key_released( uint btn ) const noexcept {
        if( obj->key.empty() ) return 1; 
        else { for( auto x : obj->key ){
            if( x == btn ) return 0;
        }}  return 1;
    }
    
    /*─······································································─*/

    bool is_button_pressed( uint btn ) const noexcept {
        if( obj->button.empty() ) return 0; 
        else { for( auto x : obj->button ){
            if( x == btn ) return 1;
        }}  return 0;
    }

    bool is_key_pressed( uint btn ) const noexcept {
        if( obj->button.empty() ) return 0; 
        else { for( auto x : obj->key ){
            if( x == btn ) return 1;
        }}  return 0;
    }
    
    /*─······································································─*/

    const ptr_t<int> size() const noexcept { return {{ XDisplayWidth(obj->dpy,0), XDisplayHeight(obj->dpy,0) }}; }

    void background( ulong color ) const noexcept { XSetWindowBackground( obj->dpy, obj->win, color ); }

    void border_w( ulong width ) const noexcept { XSetWindowBorderWidth( obj->dpy, obj->win, width ); }

    void border_c( ulong color ) const noexcept { XSetWindowBorder( obj->dpy, obj->win, color ); }

    void name( string_t name ) const noexcept { XStoreName( obj->dpy, obj->win, name.c_str() ); }

    void move( ulong x, ulong y ) const noexcept { XMoveWindow( obj->dpy, obj->win, x, y ); }

    bool is_closed() const noexcept { return obj == nullptr ? 1 : obj->state<1; }

    void resize( ulong w, ulong h ) noexcept { XFreeGC( obj->dpy, obj->ctx );
        obj->ctx = XCreateGC( obj->dpy, obj->win, 0, NULL );
        XResizeWindow( obj->dpy, obj->win, w, h ); 
    }

    void raise() const noexcept { XRaiseWindow( obj->dpy, obj->win ); }

    void flush() const noexcept { XFlush( obj->dpy ); }

    /*─······································································─*/

    virtual void force_close() const noexcept {
        if( obj->state == 0 ) { return; } obj->state = -1; onClose.emit(); 
        XFreeGC( obj->dpy,obj->ctx ); XDestroyWindow( obj->dpy,obj->win );
        XCloseDisplay( obj->dpy );
    }

    void close() const noexcept { obj->state = -1; onExit.emit(); }
    
    /*─······································································─*/

    void draw_image( int x, int y, int w, int h, char* buffer ) const noexcept {
        XImage* ximage = XCreateImage( obj->dpy, CopyFromParent, DefaultDepth(obj->dpy,DefaultScreen(obj->dpy)), ZPixmap, 0, NULL, w, h, x, y );
        XPutImage( obj->dpy, obj->win, DefaultGC(obj->dpy,DefaultScreen(obj->dpy)), ximage, 0, 0, 0, 0, w, h ); ximage->data = buffer;
    }
    
    /*─······································································─*/

    void draw_arc( ulong color, int x, int y, uint a1, uint a2, uint w, uint h, bool outline=0 ) const noexcept { 
        XSetForeground( obj->dpy, obj->ctx, color );
        if( !outline ){ XFillArc( obj->dpy, obj->win, obj->ctx, x, y, a1, a2, w, h ); } 
                 else { XDrawArc( obj->dpy, obj->win, obj->ctx, x, y, a1, a2, w, h ); }
    }

    /*─······································································─*/

    template< ulong N >
    void draw_rectangles( ulong color, const XRectangle (&rects) [N], bool outline=0 ) const noexcept {
        XSetForeground( obj->dpy, obj->ctx, color );
        if( !outline ){ XFillRectangles( obj->dpy, obj->win, obj->ctx, rects, N ); } 
                 else { XDrawRectangles( obj->dpy, obj->win, obj->ctx, rects, N ); }
    }
    
    /*─······································································─*/

    void draw_rectangle( ulong color, int x, int y, uint w, uint h, bool outline=0 ) const noexcept { 
        XSetForeground( obj->dpy, obj->ctx, color );
        if( !outline ){ XFillRectangle( obj->dpy, obj->win, obj->ctx, x, y, w, h ); } 
                 else { XDrawRectangle( obj->dpy, obj->win, obj->ctx, x, y, w, h ); }
    }

    /*─······································································─*/

    template< ulong N >
    void draw_polygon( ulong color, const XPoint (&points) [N], bool outline=0 ) const noexcept {
        XSetForeground( obj->dpy, obj->ctx, color );
        if( !outline ){ XFillPolygon( obj->dpy, obj->win, obj->ctx, points, N, CoordModeOrigin ); } 
                 else { XDrawPolygon( obj->dpy, obj->win, obj->ctx, points, N, CoordModeOrigin ); }
    }

    /*─······································································─*/

    template< ulong N >
    void draw_arcs( ulong color, const XArc (&rects) [N], bool outline=0 ) const noexcept {
        XSetForeground( obj->dpy, obj->ctx, color );
        if( !outline ){ XFillArcs( obj->dpy, obj->win, obj->ctx, rects, N ); } 
                 else { XDrawArcs( obj->dpy, obj->win, obj->ctx, rects, N ); }
    }

    /*─······································································─*/

    void draw_string( ulong color, int x, int y, string_t message ) const noexcept { 
        auto font = XLoadFont( obj->dpy, "*-13" );
        XSetForeground( obj->dpy, obj->ctx, color ); XSetFont( obj->dpy, obj->ctx, font );
        XDrawString( obj->dpy, obj->win, obj->ctx, x, y, message.c_str(), message.size() );
    }
    
    /*─······································································─*/

    void draw_line( ulong color, int x, int y, int w, int h ) const noexcept { 
        XSetForeground( obj->dpy, obj->ctx, color ); 
        XDrawLine( obj->dpy, obj->win, obj->ctx, x, y, w, h ); 
    }

    void draw_point( ulong color, int x, int y ) const noexcept { 
        XSetForeground( obj->dpy, obj->ctx, color ); 
        XDrawPoint( obj->dpy, obj->win, obj->ctx, x, y ); 
    }

    /*─······································································─*/

    void pipe() const noexcept { if( obj->state == 1 ) { return; }

        obj->dpy  = XOpenDisplay(NULL); 
        obj->id   = DefaultScreen( obj->dpy ); 
        obj->root = RootWindow( obj->dpy, obj->id );
        obj->scr  = DefaultScreenOfDisplay( obj->dpy );

        if( obj->dpy == NULL ){ _EError( onError, "can't create a canvas" ); close(); return; }
            obj->win  = XCreateSimpleWindow( obj->dpy, obj->root, obj->info.x, obj->info.y, obj->info.w, obj->info.h, 0, 0, 0 );

        auto events = FocusChangeMask | VisibilityChangeMask | StructureNotifyMask |
                      EnterWindowMask | ResizeRedirectMask   | LeaveWindowMask     |
                      ButtonPressMask | ButtonReleaseMask    | ButtonMotionMask    |
                      KeyReleaseMask  | ExposureMask         | KeyPressMask        ;
        
           obj->ctx = XCreateGC( obj->dpy, obj->win, 0, NULL );

        if( !obj->info.resize ){
            XSizeHints hints; hints.flags = PMinSize | PMaxSize;
            hints.min_height = hints.max_height = obj->info.h;
            hints.min_width = hints.max_width = obj->info.w;
            XSetWMNormalHints( obj->dpy, obj->win, &hints );
        }

        XSetStandardProperties( obj->dpy, obj->win, obj->info.name.c_str(), obj->info.name.c_str(), None, NULL, 0, NULL );
        XSelectInput( obj->dpy, obj->win, events ); XMapWindow( obj->dpy, obj->win ); flush(); obj->state = 1;

        process::loop::add([=]( canvas_t inp ){
        _Start 
        
            while( XPending(inp.obj->dpy) <= 0 ) { inp.onLoop.emit(); _Next; } XNextEvent( inp.obj->dpy, &inp.obj->event );
               if( inp.obj->event.type == ClientMessage && inp.obj->event.xclient.message_type == 258 ){ inp.close(); _End; }

    /*─······································································─*/

            elif( inp.obj->event.type == Expose )       { inp.onDraw.emit(); inp.flush(); }
            elif( inp.obj->event.type == FocusOut )     { inp.onFocusOut.emit(); }
            elif( inp.obj->event.type == FocusIn )      { inp.onFocusIn.emit(); }
            elif( inp.obj->event.type == EnterNotify )  { inp.onEnter.emit(); }
            elif( inp.obj->event.type == LeaveNotify )  { inp.onLeave.emit(); }
            elif( inp.obj->event.type == GravityNotify ){ inp.onMove.emit(); }

    /*─······································································─*/

            elif( inp.obj->event.type == ButtonRelease ) { 
                uint bt = inp.obj->event.xbutton.button;
                for( ulong x=inp.obj->button.size(); x--; ){
                    if( inp.obj->button[x] == bt ) 
                      { inp.obj->button.erase(x); }
                }   inp.onButtonRelease.emit(); 
            }

            elif( inp.obj->event.type == ButtonPress ) { 
                uint bt = inp.obj->event.xbutton.button;
                for( ulong x=inp.obj->button.size(); x--; ){
                    if( inp.obj->button[x] == bt ) 
                      { inp.onButtonPress.emit(); break; }
                }   inp.obj->button.push( bt ); inp.onButtonPress.emit();
            }

    /*─······································································─*/

            elif( inp.obj->event.type == KeyRelease ) { 
                uint bt = inp.obj->event.xkey.keycode;
                for( ulong x=inp.obj->key.size(); x--; ){
                    if( inp.obj->key[x] == bt ) 
                      { inp.obj->key.erase(x); }
                }   inp.onKeyRelease.emit(); 
            }

            elif( inp.obj->event.type == KeyPress ) { 
                uint bt = inp.obj->event.xkey.keycode;
                for( ulong x=inp.obj->key.size(); x--; ){
                    if( inp.obj->key[x] == bt ) 
                      { inp.onKeyPress.emit(); break; }
                }   inp.obj->key.push( bt ); inp.onKeyPress.emit();
            }

    /*─······································································─*/

            elif(( (ulong)inp.obj->event.xresizerequest.height != inp.obj->info.h   ||
                   (ulong)inp.obj->event.xresizerequest.width  != inp.obj->info.w ) && 
                          inp.obj->event.type == ResizeRequest 
            )   { inp.onResize.emit(); inp.onDraw.emit(); inp.flush(); }

            if( inp.obj->state == 1 ) _Goto(0); 
        
        _Stop
        }, *this );

    }

};}
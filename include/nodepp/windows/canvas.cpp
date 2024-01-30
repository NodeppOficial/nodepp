#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>
#include <winuser.h>
#include <windowsx.h>

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

        int state = 0; 
        display_t info;

        array_t<uint> button, key;
        int mouse[2] = { 0, 0 };

        WNDCLASSEX wc;
        HINSTANCE hInstance; 
        MSG msg; HWND hwnd; INPUT input;

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

    ptr_t<int> get_mouse_position() const noexcept { return {{ obj->mouse[0], obj->mouse[1] }}; }
/*
    ptr_t<ulong> get_screen_size() const noexcept { return {{ obj->scr->width, obj->scr->height }}; }

    ptr_t<int> get_screen_position() const noexcept { XWindowAttributes attr;
        XGetWindowAttributes( obj->dpy, obj->root, &attr);
        return {{ attr.x, attr.y }}; 
    }

    ptr_t<int> get_canvas_size() const noexcept { XWindowAttributes attr;
        XGetWindowAttributes( obj->dpy, obj->root, &attr);
        return {{ attr.width, attr.height }}; 
    }
*/ 
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
/*
    const ptr_t<int> size() const noexcept { return {{ XDisplayWidth(obj->dpy,0), XDisplayHeight(obj->dpy,0) }}; }

    void background( ulong color ) const noexcept { XSetWindowBackground( obj->dpy, obj->win, color ); }

    void border_w( ulong width ) const noexcept { XSetWindowBorderWidth( obj->dpy, obj->win, width ); }

    void border_c( ulong color ) const noexcept { XSetWindowBorder( obj->dpy, obj->win, color ); }

    void name( string_t name ) const noexcept { XStoreName( obj->dpy, obj->win, name.c_str() ); }

    void move( ulong x, ulong y ) const noexcept { XMoveWindow( obj->dpy, obj->win, x, y ); }
*/

    bool is_closed() const noexcept { return obj == nullptr ? 1 : obj->state<1; }

/*
    void resize( ulong w, ulong h ) noexcept { XFreeGC( obj->dpy, obj->ctx );
        obj->ctx = XCreateGC( obj->dpy, obj->win, 0, NULL );
        XResizeWindow( obj->dpy, obj->win, w, h ); 
    }

    void raise() const noexcept { XRaiseWindow( obj->dpy, obj->win ); }

    void flush() const noexcept { XFlush( obj->dpy ); }
*/
    /*─······································································─*/

    virtual void force_close() const noexcept {
        if( obj->state == 0 ) { return; } obj->state = -1;
            onClose.emit(); DestroyWindow( obj->hwnd ); 
    }

    void close() const noexcept { obj->state = -1; onExit.emit(); }
    
    /*─······································································─*/
/*
    void draw_image( int x, int y, int w, int h, char* buffer ) const noexcept {
        XImage* ximage = XCreateImage( obj->dpy, CopyFromParent, DefaultDepth(obj->dpy,DefaultScreen(obj->dpy)), ZPixmap, 0, NULL, w, h, x, y );
        XPutImage( obj->dpy, obj->win, DefaultGC(obj->dpy,DefaultScreen(obj->dpy)), ximage, 0, 0, 0, 0, w, h ); ximage->data = buffer;
    }
*/  
    /*─······································································─*/
/*
    void draw_arc( ulong color, int x, int y, uint a1, uint a2, uint w, uint h, bool outline=0 ) const noexcept { 
        XSetForeground( obj->dpy, obj->ctx, color );
        if( !outline ){ XFillArc( obj->dpy, obj->win, obj->ctx, x, y, a1, a2, w, h ); } 
                 else { XDrawArc( obj->dpy, obj->win, obj->ctx, x, y, a1, a2, w, h ); }
    }
*/
    /*─······································································─*/
/*
    template< ulong N >
    void draw_rectangles( ulong color, const XRectangle (&rects) [N], bool outline=0 ) const noexcept {
        XSetForeground( obj->dpy, obj->ctx, color );
        if( !outline ){ XFillRectangles( obj->dpy, obj->win, obj->ctx, rects, N ); } 
                 else { XDrawRectangles( obj->dpy, obj->win, obj->ctx, rects, N ); }
    }
*/
    /*─······································································─*/
/*
    void draw_rectangle( ulong color, int x, int y, uint w, uint h, bool outline=0 ) const noexcept { 
        XSetForeground( obj->dpy, obj->ctx, color );
        if( !outline ){ XFillRectangle( obj->dpy, obj->win, obj->ctx, x, y, w, h ); } 
                 else { XDrawRectangle( obj->dpy, obj->win, obj->ctx, x, y, w, h ); }
    }
*/
    /*─······································································─*/
/*
    template< ulong N >
    void draw_polygon( ulong color, const XPoint (&points) [N], bool outline=0 ) const noexcept {
        XSetForeground( obj->dpy, obj->ctx, color );
        if( !outline ){ XFillPolygon( obj->dpy, obj->win, obj->ctx, points, N, CoordModeOrigin ); } 
                 else { XDrawPolygon( obj->dpy, obj->win, obj->ctx, points, N, CoordModeOrigin ); }
    }
*/
    /*─······································································─*/
/*
    template< ulong N >
    void draw_arcs( ulong color, const XArc (&rects) [N], bool outline=0 ) const noexcept {
        XSetForeground( obj->dpy, obj->ctx, color );
        if( !outline ){ XFillArcs( obj->dpy, obj->win, obj->ctx, rects, N ); } 
                 else { XDrawArcs( obj->dpy, obj->win, obj->ctx, rects, N ); }
    }
*/
    /*─······································································─*/
/*
    void draw_string( ulong color, int x, int y, string_t message ) const noexcept { 
        auto font = XLoadFont( obj->dpy, "*-13" );
        XSetForeground( obj->dpy, obj->ctx, color ); XSetFont( obj->dpy, obj->ctx, font );
        XDrawString( obj->dpy, obj->win, obj->ctx, x, y, message.c_str(), message.size() );
    }
*/  
    /*─······································································─*/
/*
    void draw_line( ulong color, int x, int y, int w, int h ) const noexcept { 
        XSetForeground( obj->dpy, obj->ctx, color ); 
        XDrawLine( obj->dpy, obj->win, obj->ctx, x, y, w, h ); 
    }

    void draw_point( ulong color, int x, int y ) const noexcept { 
        XSetForeground( obj->dpy, obj->ctx, color ); 
        XDrawPoint( obj->dpy, obj->win, obj->ctx, x, y ); 
    }
*/
    /*─······································································─*/

    void pipe() const noexcept { if( obj->state == 1 ) { return; }
        
        // Register the window class
        obj->wc = { sizeof(WNDCLASSEXA) };
        obj->wc.lpfnWndProc   = DefWindowProc;
        obj->wc.hInstance     = GetModuleHandle(NULL);
        obj->wc.lpszClassName = obj->info.clss.c_str();

        RegisterClassExA( &obj->wc );

        // Create the window
        obj->hwnd = CreateWindowExA(
            0,                                  // Optional window styles
            obj->info.clss.c_str(),             // Window class
            obj->info.name.c_str(),             // Window text
            WS_OVERLAPPEDWINDOW,                // Window style

            CW_USEDEFAULT, CW_USEDEFAULT, 
            obj->info.w  , obj->info.h  ,

            NULL,                               // Parent window
            NULL,                               // Menu
            GetModuleHandle(NULL),              // Instance handle
            NULL                                // Additional application data
        );

        if( obj->hwnd == NULL ){ _EError( onError, "can't create a canvas" ); close(); return; }
            ShowWindow( obj->hwnd, SW_SHOWNORMAL ); UpdateWindow( obj->hwnd ); obj->state = 1;

        process::loop::add([=]( canvas_t inp ){
        _Start 

        	while( GetMessage( &inp.obj->msg, NULL, 0, 0 ) == 0 ){ _Next; }
             TranslateMessage( &inp.obj->msg ); DispatchMessage( &inp.obj->msg );

    /*─······································································─*/

            if ( !IsWindow( inp.obj->hwnd ) ){ inp.close(); _End; }

    /*─······································································─*/

			elif( inp.obj->msg.message == WM_MOUSEMOVE ) {
                  inp.obj->mouse[0] = GET_X_LPARAM( inp.obj->msg.lParam );
                  inp.obj->mouse[1] = GET_Y_LPARAM( inp.obj->msg.lParam );
                  inp.onMouseMotion.emit(); 
            }

    /*─······································································─*/

            elif( inp.obj->msg.message == WM_KEYDOWN ) { uint bt = inp.obj->msg.wParam;
                for( ulong x=inp.obj->key.size(); x--; ){
                    if( inp.obj->key[x] == bt ){ return 1; }
                }       inp.obj->key.push( bt ); inp.onKeyPress.emit();
            }

            elif( inp.obj->msg.message == WM_KEYUP ) { uint bt = inp.obj->msg.wParam;
                for( ulong x=inp.obj->key.size(); x--; ){
                    if( inp.obj->key[x] == bt ){ inp.obj->key.erase(x); }
                }       inp.onKeyRelease.emit(); 
            }

    /*─······································································─*/

            elif( inp.obj->msg.message == WM_LBUTTONDOWN ) { uint bt = 0;
                for( ulong x=inp.obj->key.size(); x--; ){
                    if( inp.obj->key[x] == bt ){ return 1; }
                }       inp.obj->key.push( bt ); inp.onKeyPress.emit();
            }

            elif( inp.obj->msg.message == WM_LBUTTONUP ) { uint bt = 0;
                for( ulong x=inp.obj->key.size(); x--; ){
                    if( inp.obj->key[x] == bt ){ inp.obj->key.erase(x); }
                }       inp.onKeyRelease.emit(); 
            }

    /*─······································································─*/

            elif( inp.obj->msg.message == WM_RBUTTONDOWN ) { uint bt = 1;
                for( ulong x=inp.obj->key.size(); x--; ){
                    if( inp.obj->key[x] == bt ){ return 1; }
                }       inp.obj->key.push( bt ); inp.onKeyPress.emit();
            }

            elif( inp.obj->msg.message == WM_RBUTTONUP ) { uint bt = 1;
                for( ulong x=inp.obj->key.size(); x--; ){
                    if( inp.obj->key[x] == bt ){ inp.obj->key.erase(x); }
                }       inp.onKeyRelease.emit(); 
            }

    /*─······································································─*/

            elif( inp.obj->msg.message == WM_MBUTTONDOWN ) { uint bt = 3;
                for( ulong x=inp.obj->key.size(); x--; ){
                    if( inp.obj->key[x] == bt ){ return 1; }
                }       inp.obj->key.push( bt ); inp.onKeyPress.emit();
            }

            elif( inp.obj->msg.message == WM_MBUTTONUP ) { uint bt = 3;
                for( ulong x=inp.obj->key.size(); x--; ){
                    if( inp.obj->key[x] == bt ){ inp.obj->key.erase(x); }
                }       inp.onKeyRelease.emit(); 
            }

    /*─······································································─*/
        
            if( inp.obj->state == 1 ) _Goto(0); 
        
        _Stop
        }, *this );

    }

};}
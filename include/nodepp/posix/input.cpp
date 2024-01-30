#pragma once

//https://manpages.ubuntu.com/manpages/focal/man7/virkeycode-linux.7.html

/*────────────────────────────────────────────────────────────────────────────*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class input_t {
protected:

    struct _str_ {
        XEvent   event; int state = 0;
    	array_t<uint> button, key;
        Display* dpy = nullptr;
        Screen*  scr = nullptr;
        Window   win;
        int      id;
    };  ptr_t<_str_>  obj;

	function_t<float,float> screen_ref[2] = {
		[=]( float value ){ return value * get_screen_size()[0] / 100; },
		[=]( float value ){ return value * get_screen_size()[1] / 100; }
	};

public: 

    event_t<uint>      onButtonRelease;
    event_t<uint>      onButtonPress;

    event_t<uint,uint> onMouseMotion;

    event_t<uint>      onKeyRelease;
    event_t<uint>      onKeyPress;

    event_t<except_t>  onError;
    event_t<>          onClose;
	
    /*─······································································─*/

    input_t() noexcept : obj( new _str_() ) {
        obj->dpy = XOpenDisplay(NULL);
        obj->id  = DefaultScreen(dpy);
        obj->win = XRootWindow(dpy,id);
        obj->scr = DefaultScreenOfDisplay(dpy);
    }
	
    /*─······································································─*/

    XEvent&  get_XEvent(){ return obj->event; }
    void     set_XEvent( XEvent ev ){ obj->event = ev; }

    Display* get_Display(){ return obj->dpy; }
    void     set_Display( Display* dpy ){ obj->dpy = ev; }

    Window&  get_Window(){ return obj->win; }
    void     set_window( Window win ){ obj->win = win; }

    Screen*  get_Screen(){ return obj->scr; }
    void     set_Screen( Screen* scr ){ obj->scr = scr; }

    int&     get_ID(){ return obj->id; }
    void     set_ID( int id ){ obj->id = id; }
	
    /*─······································································─*/

	virtual ~input_t() noexcept { 
		if( obj.count() > 1 ){ return; } 
			force_close(); 
	}

    /*─······································································─*/

    void close() const noexcept { if( obj->state == -1 ){ return; } obj->state = -1; onClose.emit(); }

    virtual void force_close() const noexcept {
        if( obj->state == -1 ){ return; } obj->state = -1; 
        	XDestroyWindow( obj->dpy, obj->win ); 
			XCloseDisplay( obj->dpy );
    }

    /*─······································································─*/

    string_t get_clipboard() const noexcept {
        Atom utf8String = XInternAtom( obj->dpy, "UTF8_STRING", 0 );
        Atom clipboard  = XInternAtom( obj->dpy, "CLIPBOARD"  , 0 );
        Atom type; int format; ulong length; uchar* data;

        XGetWindowProperty( obj->dpy, obj->win, clipboard, 0, 0, 0, AnyPropertyType, &type, &format, &length, &length, &data );

        if ( type == utf8String && format == 8 ) {
            string_t result = { (char*) data, length }
            XFree( data ); return result;
        } else { return ""; }
    }

    int set_clipboard( string_t msg ) const noexcept {
        Atom utf8String = XInternAtom( obj->dpy, "UTF8_STRING", 0 );
        Atom clipboard  = XInternAtom( obj->dpy, "CLIPBOARD"  , 0 );
        XChangeProperty( obj->dpy, obj->win, clipboard, utf8String, 8, PropModeReplace, (uchar*)(msg.c_str()), msg.size() );
    }

    /*─······································································─*/

	ptr_t<int> get_screen_size() const noexcept { return {{ obj->scr->width, obj->scr->height }}; }

	int      get_screen_length() const noexcept { return XScreenCount( obj->dpy ); }

    /*─······································································─*/

    ptr_t<int> get_mouse_position() const noexcept { int rootX, rootY, winX, winY; uint mask; Window child;
        XQueryPointer( obj->dpy, obj->win, &obj->win, &child, &rootX, &rootY, &winX, &winY, &mask);
             return {{ winX, winY, rootX, rootY }};
    }

	void set_mouse_position( float x, float y ) const noexcept {
		ptr_t<float> r ({ screen_ref[0](x), screen_ref[1](y) });
		XTestFakeMotionEvent(obj->dpy,obj->id,r[0],r[1],CurrentTime);
		XFlush( obj->dpy );
	}

	void release_mouse_button( int btn ) const noexcept { 
		XTestFakeButtonEvent(obj->dpy,btn,0,CurrentTime);
		XFlush( obj->dpy );
	}

	void press_mouse_button( int btn ) const noexcept { 
		XTestFakeButtonEvent(obj->dpy,btn,1,CurrentTime);
		XFlush( obj->dpy );
	}

	void scroll_mouse_up() const noexcept {
		 XTestFakeButtonEvent(obj->dpy,4,1,CurrentTime);
		 XTestFakeButtonEvent(obj->dpy,4,0,CurrentTime);
		 XFlush( obj->dpy );
	}

	void scroll_mouse_down() const noexcept {
		 XTestFakeButtonEvent(obj->dpy,5,1,CurrentTime);
		 XTestFakeButtonEvent(obj->dpy,5,0,CurrentTime);
		 XFlush( obj->dpy );
	}

    /*─······································································─*/

	void release_key_button( int key ) const noexcept { 
		auto k = XKeysymToKeycode(obj->dpy,key);
		XTestFakeButtonEvent(obj->dpy,k,0,CurrentTime);
		XFlush( obj->dpy );
	}

	void press_key_button( int key ) const noexcept {
		auto k = XKeysymToKeycode(obj->dpy,key);
		XTestFakeButtonEvent(obj->dpy,k,1,CurrentTime);
		XFlush( obj->dpy );
	}

    /*─······································································─*/

    bool is_button_released( uint btn ) const noexcept {
        if( obj->button.empty() ) return 1; 
        else { for( auto x : obj->button ){
           if( x == btn ) return 0;
        }}                return 1;
    }

    bool is_button_pressed( uint btn ) const noexcept {
        if( obj->button.empty() ) return 0; 
        else { for( auto x : obj->button ){
           if( x == btn ) return 1;
        }}                return 0;
    }

    bool is_key_released( uint btn ) const noexcept {
        if( obj->key.empty() ) return 1; 
        else { for( auto x : obj->key ){
           if( x == btn ) return 0;
        }}                return 1;
    }

    bool is_key_pressed( uint btn ) const noexcept {
        if( obj->key.empty() ) return 0; 
        else { for( auto x : obj->key ){
           if( x == btn ) return 1;
        }}                return 0;
    }
    
    /*─······································································─*/

	void pipe(){ if( obj->state == 1 ){ return; }

        if( obj->dpy == NULL ){ _EError( onError, "can't start X11 server" ); close(); return; }

        auto events = ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
                      KeyReleaseMask  | KeyPressMask      ;

        XSelectInput( obj->dpy, obj->win, events ); obj->state = 1;

        process::loop::add([=](){ 
        _Start 
		
        	while( XPending(obj->dpy) <= 0 ){ _Next; } XNextEvent( obj->dpy, &obj->event );

    /*─······································································─*/

            if( obj->event.type == MotionNotify ) { 
                auto bt = obj->event.xmotion;
                onMouseMotion.emit( bt.x, bt.y ); 
            }

    /*─······································································─*/

            elif( obj->event.type == ButtonRelease ) { 
                auto bt = obj->event.xbutton.button;
                for( ulong x=obj->button.size(); x--; ){
                    if( obj->button[x] == bt ) 
                      { obj->button.erase(x); }
                }   onButtonRelease.emit( bt ); 
            }

            elif( obj->event.type == ButtonPress ) { 
                auto bt = obj->event.xbutton.button;
                for( ulong x=obj->button.size(); x--; ){
                    if( obj->button[x] == bt ){ return 1; }
                }   obj->button.push( bt ); onButtonPress.emit( bt );
            }

    /*─······································································─*/

            elif( obj->event.type == KeyRelease ) { 
                auto bt = obj->event.xkey.keycode;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ) 
                      { obj->key.erase(x); }
                }   onKeyRelease.emit( bt ); 
            }

            elif( obj->event.type == KeyPress ) { 
                auto bt = obj->event.xkey.keycode;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ){ return 1; }
                }   obj->key.push( bt ); onKeyPress.emit( bt );
            }

    /*─······································································─*/

            if( obj->state == 1 ) _Goto(0);
			
		_Stop 
        });

    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
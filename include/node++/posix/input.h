#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class simkey_t {
protected:

    struct _str_ {
    	array_t<uint> button, key;
        XEvent   event; int state = 0;
        Display* dpy = XOpenDisplay(NULL);
        int      id  = DefaultScreen(dpy);
        Window   win = XRootWindow(dpy,id);
        Screen*  scr = DefaultScreenOfDisplay(dpy);
    };  ptr_t<_str_>  obj;

	function_t<float,float> screen_ref[2] = {
		[=]( float value ){ return value * get_screen_size()[0] / 100; },
		[=]( float value ){ return value * get_screen_size()[1] / 100; }
	};

public: simkey_t() noexcept : obj(new _str_) {}

    event_t<uint>      onButtonRelease;
    event_t<uint>      onButtonPress;

    event_t<uint,uint> onMouseMotion;

    event_t<uint>      onKeyRelease;
    event_t<uint>      onKeyPress;

    event_t<except_t>  onError;
    event_t<>          onClose;
	
    /*─······································································─*/

	virtual ~simkey_t() noexcept { 
		if( obj.count() > 1 ){ return; } 
			force_close(); 
	}

    /*─······································································─*/

    ptr_t<int> get_mouse_position() const noexcept { int rootX, rootY, winX, winY; uint mask; Window child;
        XQueryPointer( obj->dpy, obj->win, &obj->win, &child, &rootX, &rootY, &winX, &winY, &mask);
        return {{ winX, winY, rootX, rootY }};
    }

    void close() const noexcept { if( obj->state == -1 ){ return; } obj->state = -1; onClose.emit(); }

    virtual void force_close() const noexcept {
        if( obj->state == -1 ){ return; } obj->state = -1; 
        	XDestroyWindow( obj->dpy, obj->win ); 
			XCloseDisplay( obj->dpy );
    }

    /*─······································································─*/

	ptr_t<int> get_screen_size() const noexcept { return {{ obj->scr->width, obj->scr->height }}; }

	int      get_screen_length() const noexcept { return XScreenCount( obj->dpy ); }

    /*─······································································─*/

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

        if( obj->dpy == NULL ){ $onError( onError, "can't start X11 server" ); close(); return; }

        auto events = ButtonPressMask | ButtonReleaseMask   | ButtonMotionMask   |
                      KeyReleaseMask  | KeyPressMask        ;

        XSelectInput( obj->dpy, obj->win, events ); obj->state = 1;

        process::loop::add([=](){ $Start 
			while( XPending(obj->dpy) <= 0 ){ $Next; } XNextEvent( obj->dpy, &obj->event );

    /*─······································································─*/

            if( obj->event.type == MotionNotify ) { 
                auto bt = obj->event.xmotion;
                onMouseMotion.emit( bt.x, bt.y ); 
            }

    /*─······································································─*/

            else if( obj->event.type == ButtonRelease ) { 
                auto bt = obj->event.xbutton.button;
                for( ulong x=obj->button.size(); x--; ){
                    if( obj->button[x] == bt ) 
                      { obj->button.erase(x); }
                }   onButtonRelease.emit( bt ); 
            }

            else if( obj->event.type == ButtonPress ) { 
                auto bt = obj->event.xbutton.button;
                for( ulong x=obj->button.size(); x--; ){
                    if( obj->button[x] == bt ){ return 1; }
                }   obj->button.push( bt ); onButtonPress.emit( bt );
            }

    /*─······································································─*/

            else if( obj->event.type == KeyRelease ) { 
                auto bt = obj->event.xkey.keycode;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ) 
                      { obj->key.erase(x); }
                }   onKeyRelease.emit( bt ); 
            }

            else if( obj->event.type == KeyPress ) { 
                auto bt = obj->event.xkey.keycode;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ){ return 1; }
                }   obj->key.push( bt ); onKeyPress.emit( bt );
            }

    /*─······································································─*/

            if( obj->state == 1 ) $Goto(0);
			
		$Stop });

    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
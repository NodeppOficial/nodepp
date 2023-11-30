#ifndef NODEPP_SIMKEY
#define NODEPP_SIMKEY

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef linux
#include <windows.h>
#include <winuser.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class simkey_t {
protected:

    struct _str_ {
		MSG event;
        INPUT input; int state=0;
    	array_t<uint> button, key;
    };  ptr_t<_str_> obj = new _str_();

	function_t<float,float> screen_ref[2] = {
		[=]( float value ){ return value * get_screen_size()[0] / 100; },
		[=]( float value ){ return value * get_screen_size()[1] / 100; }
	};

public: simkey_t(){}

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

    bool is_closed() const noexcept { return obj==nullptr ? 1 : obj->state==-1; }

    virtual void force_close() const noexcept {
        if( obj->state == -1 ){ return; } 
			obj->state =  -1;
    }

    void close() const noexcept { obj->state = -1; }

    /*─······································································─*/

	int get_screen_length() const noexcept { 
		int count = 0; auto cb = [&]( ... ){ return true; };
		return ::EnumDisplayMonitors( NULL, NULL, cb, (LPARAM)&count ) ? count : 0;
	}

	ptr_t<int> get_screen_size() const noexcept { return {{
		::GetSystemMetrics( SM_CYSCREEN ),
		::GetSystemMetrics( SM_CXSCREEN )
	}}; }

    /*─······································································─*/

	void scroll_mouse_down() const noexcept { mouse_event( MOUSEEVENTF_WHEEL, 0, 0, 120, 0 ); }

	void   scroll_mouse_up() const noexcept { mouse_event( MOUSEEVENTF_WHEEL, 0, 0,-120, 0 ); }

	void mouse_position( float x, float y ) const noexcept {
		auto dx = screen::screen_ref[0](x) * ( 65535.0f / screen::screen[0] );
		auto dy = screen::screen_ref[1](y) * ( 65535.0f / screen::screen[1] );
		auto fg = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		mouse_event( fg, dx, dy, 0, 0 );
	}

	void release_mouse_button( int btn ) const noexcept { int fg = 0;
		input.type = INPUT_MOUSE; switch( btn ){
			case 3: fg = MOUSEEVENTF_MIDDLEUP; break;
			case 2: fg = MOUSEEVENTF_RIGHTUP;  break;
			case 1: fg = MOUSEEVENTF_LEFTUP;   break;
		} 	mouse_event( fg, 0, 0, 1, 0 ); 
	}

	void press_mouse_button( int btn ) const noexcept { int fg = 0;
		input.type = INPUT_MOUSE; switch( btn ){
			case 3: fg = MOUSEEVENTF_MIDDLEDOWN; break;
			case 2: fg = MOUSEEVENTF_RIGHTDOWN;  break;
			case 1: fg = MOUSEEVENTF_LEFTDOWN;   break;
		} 	mouse_event( fg, 0, 0, 1, 0 ); 
	}
    
    /*─······································································─*/

	void release_key_button( int key ) const noexcept { 
		 keybd_event( key, 0x45, 0, 0 ); 
	}

	void press_key_button( int key ) const noexcept { 
		 keybd_event( key, 0x45, 2, 0 ); 
	}
    
    /*─······································································─*/

    bool is_button_released( uint btn ) const noexcept {
        if( obj->button.empty() ) return 1; 
        else { for( auto x : obj->button ){
           	if( x == btn ) return 0;
        }}  return 1;
    }

    bool is_button_pressed( uint btn ) const noexcept {
        if( obj->button.empty() ) return 0; 
        else { for( auto x : obj->button ){
           	if( x == btn ) return 1;
        }}  return 0;
    }

    bool is_key_released( uint btn ) const noexcept {
        if( obj->key.empty() ) return 1; 
        else { for( auto x : obj->key ){
           	if( x == btn ) return 0;
        }}                 return 1;
    }

    bool is_key_pressed( uint btn ) const noexcept {
        if( obj->key.empty() ) return 0; 
        else { for( auto x : obj->key ){
           	if( x == btn ) return 1;
        }}                 return 0;
    }
    
    /*─······································································─*/

	void pipe(){ if( obj->state == 1 ){ return; }

        if( obj->dpy == NULL ){ _onError( onError, "can't create a canvas" ); close(); return; }

        process::loop::add([=](){ _Start 
			while( GetMessage( &obj->msg, NULL, 0, 0 ) == 0 ){ _Next; }
			TranslateMessage( &obj->msg ); DispatchMessage( &obj->msg );

    /*─······································································─*/

			if( msg.message == WM_MOUSEMOVE ){
                int btX = GET_X_LPARAM( msg.lParam );
                int btY = GET_Y_LPARAM( msg.lParam );
                	onMouseMotion.emit( btX, btY ); 
            }

    /*─······································································─*/

    /*─······································································─*/

            else if( msg.message == WM_KEYDOWN ) { auto bt = wParam;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ){ return 1; }
                }   obj->key.push( bt ); onKeyPress.emit( bt );
            }

            else if( msg.message == WM_KEYUP ) { auto bt = wParam;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ) 
                      { obj->key.erase(x); }
                }   onKeyRelease.emit( bt ); 
            }

    /*─······································································─*/

            if( obj->state == 1 ) _Goto(0);
			
		_Stop });

    }

};}

#else

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

namespace nodepp { class simkey_t {
protected:

    struct _str_ {
    	array_t<uint> button, key;
        XEvent   event; int state = 0;
        Display* dpy = XOpenDisplay(NULL);
        int      id  = DefaultScreen(dpy);
        Window   win = XRootWindow(dpy,id);
        Screen*  scr = DefaultScreenOfDisplay(dpy);
    };  ptr_t<_str_>  obj = new _str_();

	function_t<float,float> screen_ref[2] = {
		[=]( float value ){ return value * get_screen_size()[0] / 100; },
		[=]( float value ){ return value * get_screen_size()[1] / 100; }
	};

public: simkey_t(){}

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
        }}  return 1;
    }

    bool is_button_pressed( uint btn ) const noexcept {
        if( obj->button.empty() ) return 0; 
        else { for( auto x : obj->button ){
           	if( x == btn ) return 1;
        }}  return 0;
    }

    bool is_key_released( uint btn ) const noexcept {
        if( obj->key.empty() ) return 1; 
        else { for( auto x : obj->key ){
           	if( x == btn ) return 0;
        }}                 return 1;
    }

    bool is_key_pressed( uint btn ) const noexcept {
        if( obj->key.empty() ) return 0; 
        else { for( auto x : obj->key ){
           	if( x == btn ) return 1;
        }}                 return 0;
    }
    
    /*─······································································─*/

	void pipe(){ if( obj->state == 1 ){ return; }

        if( obj->dpy == NULL ){ _onError( onError, "can't start X11 server" ); close(); return; }

        auto events = ButtonPressMask | ButtonReleaseMask   | ButtonMotionMask   |
                      KeyReleaseMask  | KeyPressMask        ;

        XSelectInput( obj->dpy, obj->win, events ); obj->state = 1;

        process::loop::add([=](){ _Start 
			while( XPending(obj->dpy) <= 0 ){ _Next; } XNextEvent( obj->dpy, &obj->event );

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

            if( obj->state == 1 ) _Goto(0);
			
		_Stop });

    }

};}

#endif
#endif
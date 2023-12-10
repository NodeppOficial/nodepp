#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>
#include <winuser.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class simkey_t {
protected:

    struct _str_ {   MSG event;
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

        if( obj->dpy == NULL ){ $onError( onError, "can't create a canvas" ); close(); return; }

        process::loop::add([=](){ $Start 
			while( GetMessage( &obj->msg, NULL, 0, 0 ) == 0 ){ $Next; }
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

            if( obj->state == 1 ) $Goto(0);
			
		_Stop });

    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
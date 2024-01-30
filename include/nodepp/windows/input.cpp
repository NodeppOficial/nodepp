#pragma once

//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>
#include <winuser.h>
#include <windowsx.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class input_t {
protected:

    struct _str_ {    MSG msg;
    	array_t<uint> button, key;
        INPUT input; int state=0;
    };  ptr_t<_str_> obj;

	function_t<float,float> screen_ref[2] = {
		[=]( float value ){ return value * get_screen_size()[0] / 100; },
		[=]( float value ){ return value * get_screen_size()[1] / 100; }
	};

public: input_t() noexcept : obj( new _str_() ) {}

    event_t<uint>      onButtonRelease;
    event_t<uint>      onButtonPress;

    event_t<uint,uint> onMouseMotion;

    event_t<uint>      onKeyRelease;
    event_t<uint>      onKeyPress;

    event_t<except_t>  onError;
    event_t<>          onClose;

    /*─······································································─*/

	virtual ~input_t() noexcept { 
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

    string_t get_clipboard() const noexcept { string_t s;

        if ( !OpenClipboard(nullptr) ) { return s; }

        HANDLE clipboardData = GetClipboardData( CF_TEXT );
        if( clipboardData != nullptr ) {
            char* data = (char*) GlobalLock( clipboardData );
            if( data != nullptr ) { s += data;
                GlobalUnlock( clipboardData );
            }
        }

        CloseClipboard(); return s;
    }

    int set_clipboard( string_t msg ) const noexcept {

        if( !OpenClipboard(nullptr) ) { return -1; }

        HGLOBAL clipboardData = GlobalAlloc( GMEM_MOVEABLE, msg.size() );
        if( clipboardData != nullptr ) {
            char* data = (char*) GlobalLock( clipboardData );
            if( data != nullptr ) {
                strcpy( data, msg.c_str() );
                GlobalUnlock( clipboardData );
                SetClipboardData( CF_TEXT, clipboardData );
            }
        }

        CloseClipboard(); return 0;
    }

    /*─······································································─*/

	int get_screen_length() const noexcept { return GetSystemMetrics(SM_CMONITORS); }

	ptr_t<int> get_screen_size() const noexcept { 
        return {{
		    ::GetSystemMetrics( SM_CYSCREEN ),
		    ::GetSystemMetrics( SM_CXSCREEN )
	    }}; 
    }

    /*─······································································─*/

	void scroll_mouse_down() const noexcept { mouse_event( MOUSEEVENTF_WHEEL, 0, 0, 120, 0 ); }

	void   scroll_mouse_up() const noexcept { mouse_event( MOUSEEVENTF_WHEEL, 0, 0,-120, 0 ); }

	void set_mouse_position( float x, float y ) const noexcept {
		auto fg = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		auto dx = screen_ref[0](x) * ( 65535.0f / get_screen_size()[0] );
		auto dy = screen_ref[1](y) * ( 65535.0f / get_screen_size()[1] );
		mouse_event( fg, dx, dy, 0, 0 );
	}

    ptr_t<int> get_mouse_position() const noexcept { 
        POINT mousePos; if (!GetCursorPos(&mousePos) ) { return nullptr; }
        return {{ mousePos.x, mousePos.y, mousePos.x, mousePos.y }};
    }

	void release_mouse_button( int btn ) const noexcept { int fg = 0;
		obj->input.type = INPUT_MOUSE; switch( btn ){
			case 3: fg = MOUSEEVENTF_MIDDLEUP; break;
			case 2: fg = MOUSEEVENTF_RIGHTUP;  break;
			case 1: fg = MOUSEEVENTF_LEFTUP;   break;
		} 	mouse_event( fg, 0, 0, 1, 0 ); 
	}

	void press_mouse_button( int btn ) const noexcept { int fg = 0;
		obj->input.type = INPUT_MOUSE; switch( btn ){
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

        if( this->is_closed() ){ _EError( onError, "can't start Winapi server" ); close(); return; }

        process::loop::add([=](){ 
        _Start 
		
        	while( GetMessage( &obj->msg, NULL, 0, 0 ) == 0 ){ _Next; }
			TranslateMessage( &obj->msg ); DispatchMessage( &obj->msg );

    /*─······································································─*/

			if( obj->msg.message == WM_MOUSEMOVE ) {
                int btX = GET_X_LPARAM( obj->msg.lParam );
                int btY = GET_Y_LPARAM( obj->msg.lParam );
                	onMouseMotion.emit( btX, btY ); 
            }

    /*─······································································─*/

            elif( obj->msg.message == WM_KEYDOWN ) { auto bt = obj->msg.wParam;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ){ return 1; }
                }   obj->key.push( bt ); onKeyPress.emit( bt );
            }

            elif( obj->msg.message == WM_KEYUP ) { auto bt = obj->msg.wParam;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ) 
                      { obj->key.erase(x); }
                }   onKeyRelease.emit( bt ); 
            }

    /*─······································································─*/

            elif( obj->msg.message == WM_LBUTTONDOWN ) { auto bt = 0;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ){ return 1; }
                }   obj->key.push( bt ); onKeyPress.emit( bt );
            }

            elif( obj->msg.message == WM_LBUTTONUP ) { auto bt = 0;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ) 
                      { obj->key.erase(x); }
                }   onKeyRelease.emit( bt ); 
            }

    /*─······································································─*/

            elif( obj->msg.message == WM_RBUTTONDOWN ) { auto bt = 1;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ){ return 1; }
                }   obj->key.push( bt ); onKeyPress.emit( bt );
            }

            elif( obj->msg.message == WM_RBUTTONUP ) { auto bt = 1;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ) 
                      { obj->key.erase(x); }
                }   onKeyRelease.emit( bt ); 
            }

    /*─······································································─*/

            elif( obj->msg.message == WM_MBUTTONDOWN ) { auto bt = 3;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ){ return 1; }
                }   obj->key.push( bt ); onKeyPress.emit( bt );
            }

            elif( obj->msg.message == WM_MBUTTONUP ) { auto bt = 3;
                for( ulong x=obj->key.size(); x--; ){
                    if( obj->key[x] == bt ) 
                      { obj->key.erase(x); }
                }   onKeyRelease.emit( bt ); 
            }

    /*─······································································─*/

            if( obj->state == 1 ) _Goto(0);
			
		_Stop 
        });

    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
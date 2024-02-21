#pragma once

//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

/*
#define K_LBUTTON  0x01
#define K_RBUTTON  0x02
#define K_CANCEL   0x03
#define K_MBUTTON  0x04
#define K_XBUTTON1 0x05
#define K_XBUTTON2 0x06

#define K_BACK     0x08
#define K_TAB      0x09
#define K_CLEAR    0x0C
#define K_RETURN   0x0D
#define K_SHIFT    0x10
#define K_CONTROL  0x11
#define K_MENU     0x12
#define K_PAUSE    0x13
#define K_CAPITAL  0x14
#define K_ESCAPE   0x1B

#define K_KANA     0x15
#define K_HANGUL   0x15
#define K_JUNGA    0x17
#define K_FINAL    0x18
#define K_HANJA    0x19
#define K_KANJI    0x19

#define K_IME_PRSS 0xE5
#define K_IME_ON   0x16
#define K_IME_OFF  0x1A

#define K_CONVER   0x1C
#define K_NCONVER  0x1D
#define K_ACCEPT   0x1E

#define K_0        0x30
#define K_1        0x31
#define K_2        0x32
#define K_3        0x33
#define K_4        0x34
#define K_5        0x35
#define K_6        0x36
#define K_7        0x37
#define K_8        0x38
#define K_9        0x39

#define K_A        0x41
#define K_B        0x42
#define K_C        0x43
#define K_D        0x44
#define K_E        0x45
#define K_F        0x46
#define K_G        0x47
#define K_H        0x48
#define K_I        0x49
#define K_J        0x4A
#define K_K        0x4B
#define K_L        0x4C
#define K_M        0x4D
#define K_N        0x4E
#define K_O        0x4F
#define K_P        0x50
#define K_Q        0x51
#define K_R        0x52
#define K_S        0x53
#define K_T        0x54
#define K_U        0x55
#define K_V        0x56
#define K_W        0x57
#define K_X        0x58
#define K_Y        0x59
#define K_Z        0x5A

#define K_F1       0x70
#define K_F2       0x71
#define K_F3       0x72
#define K_F4       0x73
#define K_F5       0x74
#define K_F6       0x75
#define K_F7       0x76
#define K_F8       0x77
#define K_F9       0x78
#define K_F10      0x79
#define K_F11      0x7A
#define K_F12      0x7B
#define K_F13      0x7C
#define K_F14      0x7D
#define K_F15      0x7E
#define K_F16      0x7F
#define K_F17      0x80
#define K_F18      0x81
#define K_F19      0x82
#define K_F20      0x83
#define K_F21      0x84
#define K_F22      0x85
#define K_F23      0x86
#define K_F24      0x87

#define K_MODECH   0x1F
#define K_SPACE    0x20
#define K_PRIOR    0x21
#define K_NEXT     0x22
#define K_END      0x23
#define K_HOME     0x24

#define K_LEFT     0x25
#define K_UP       0x26
#define K_RIGHT    0x27
#define K_DOWN     0x28
#define K_SELECT   0x29
#define K_PRINT    0x2A
#define K_EXECUTE  0x2B
#define K_SNAPSHOT 0x2C
#define K_INSERT   0x2D
#define K_DELETE   0x2E
#define K_HELP     0x2F

#define K_MENU     0x5B
#define K_MENU1    0x5C
#define K_MENU2    0x5D

#define K_SLEEP    0x5F
#define K_N0       0x60
#define K_N1       0x61
#define K_N2       0x62
#define K_N3       0x63
#define K_N4       0x64
#define K_N5       0x65
#define K_N6       0x66
#define K_N7       0x67
#define K_N8       0x68
#define K_N9       0x69
#define K_NMUL     0x6A
#define K_NADD     0x6B
#define K_NSEP     0x6C
#define K_NSUB     0x6D
#define K_NDEC     0x6E
#define K_NDIV     0x6F
#define K_NLOCK    0x90

#define K_SCROLL   0x91
#define K_LSHIFT   0xA0
#define K_RSHIFT   0xA1
#define K_LCNTRL   0xA2
#define K_RCNTRL   0xA3
#define K_LALT     0xA4
#define K_RALT     0xA5

#define K_BBACK    0xA6
#define K_BFORW    0xA7
#define K_BRFRS    0xA8
#define K_BSTOP    0xA9
#define K_BSRCH    0xAA
#define K_BFVRT    0xAB
#define K_BHOME    0xAC

#define K_VMUTE    0xAD
#define K_VDOWN    0xAE
#define K_VUP      0xAF

#define K_MNEXT    0xB0
#define K_MPREV    0xB1
#define K_MSTOP    0xB2
#define K_MPLAY    0xB3
*/

/*
VK_LAUNCH_MAIL	0xB4	Start Mail key
VK_LAUNCH_MEDIA_SELECT	0xB5	Select Media key
VK_LAUNCH_APP1	0xB6	Start Application 1 key
VK_LAUNCH_APP2	0xB7	Start Application 2 key

VK_OEM_1	0xBA	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ;: key
VK_OEM_PLUS	0xBB	For any country/region, the + key
VK_OEM_COMMA	0xBC	For any country/region, the , key
VK_OEM_MINUS	0xBD	For any country/region, the - key
VK_OEM_PERIOD	0xBE	For any country/region, the . key
VK_OEM_2	0xBF	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the /? key
VK_OEM_3	0xC0	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the `~ key

VK_OEM_4	0xDB	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the [{ key
VK_OEM_5	0xDC	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \\| key
VK_OEM_6	0xDD	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ]} key
VK_OEM_7	0xDE	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '" key
VK_OEM_8	0xDF	Used for miscellaneous characters; it can vary by keyboard.

VK_OEM_102	0xE2	The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard


VK_PACKET	0xE7	Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP

VK_ATTN	0xF6	Attn key

VK_CRSEL	0xF7	CrSel key
VK_EXSEL	0xF8	ExSel key
VK_EREOF	0xF9	Erase EOF key

VK_PLAY	0xFA	Play key
VK_ZOOM	0xFB	Zoom key

VK_PA1	0xFD	PA1 key
VK_OEM_CLEAR	0xFE	Clear key
*/

/*────────────────────────────────────────────────────────────────────────────*/

#include <windows.h>
#include <winuser.h>
#include <windowsx.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class input_t {
protected:

    struct image_t {
        ptr_t<char> data;
        int width, height;
    };

    struct NODE {
    	array_t<uint> button, key;
        INPUT input; int state =0; 
        MSG   msg;
    };  ptr_t<NODE> obj;

    ptr_t<float> screen_ref( const float& x, const float& y ) const noexcept{
        auto size = get_screen_size(); return {{
            x * size[0] / 100, y * size[1] / 100
        }};
	};

public: input_t() noexcept : obj( new NODE() ) {}

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

    void close() const noexcept { if( obj->state == -1 ){ return; } obj->state = -1; onClose.emit(); }

    bool is_closed() const noexcept { return obj==nullptr ? 1 : obj->state==-1; }

    virtual void force_close() const noexcept {
        if( obj->state == -1 ){ return; } 
			obj->state =  -1;
    }

    /*─······································································─*/

    image_t take_screenshot() noexcept {
        HDC hdcScreen     = GetDC( NULL );
        auto size         = get_screen_size();
        HDC hdcMemDC      = CreateCompatibleDC( hdcScreen );
        HBITMAP hbmScreen = CreateCompatibleBitmap( hdcScreen, size[0], size[1] );
                            SelectObject( hdcMemDC, hbmScreen );

        BitBlt( hdcMemDC, 0, 0, size[0], size[1], hdcScreen, 0, 0, SRCCOPY );

        BITMAPINFOHEADER bi = {0}; 
        bi.biSize           = sizeof(BITMAPINFOHEADER);
        bi.biSizeImage      =  size[0] * size[1] * 3;
        bi.biWidth          =  size[0];
        bi.biHeight         = -size[1];
        bi.biCompression    = BI_RGB;
        bi.biBitCount       = 24;
        bi.biPlanes         = 1;
        bi.biXPelsPerMeter  = 0;
        bi.biYPelsPerMeter  = 0;
        bi.biClrUsed        = 0;
        bi.biClrImportant   = 0;

        int bytesPerRow = (( size[0] * 24 + 31 ) / 32 ) * 4;
        int imageSize   = sizeof(bi) + bytesPerRow * size[1];
        ptr_t<char> data ( imageSize, 0 ); memcpy( &data, &bi, sizeof(bi) );

        GetDIBits( hdcMemDC, hbmScreen, 0, size[1], &data+sizeof(bi), (BITMAPINFO*)&bi, DIB_RGB_COLORS );
        DeleteObject( hbmScreen ); DeleteDC( hdcMemDC ); ReleaseDC( NULL, hdcScreen );

        image_t image; 
                image.data   = data;
                image.width  = size[0];
                image.height = size[1];

        return image;
    }

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
		    ::GetSystemMetrics( SM_CXSCREEN ),
		    ::GetSystemMetrics( SM_CYSCREEN )
	    }}; 
    }

    /*─······································································─*/

	void scroll_mouse_down() const noexcept { mouse_event( MOUSEEVENTF_WHEEL, 0, 0, 120, 0 ); }

	void   scroll_mouse_up() const noexcept { mouse_event( MOUSEEVENTF_WHEEL, 0, 0,-120, 0 ); }

	void set_mouse_position( float x, float y ) const noexcept {
		auto fg = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
        auto sr = screen_ref( x, y );
        auto sz = get_screen_size( );
		auto dx = sr[0] * ( 65535.0f / sz[0] );
		auto dy = sr[1] * ( 65535.0f / sz[1] );
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

	void pipe(){ if( obj->state == 1 ){ return; } auto inp = type::bind( this );

        if( is_closed() )
          { process::error( onError, "can't start Winapi server" ); close(); return; }

        process::loop::add([=](){ 
        coStart 
		
        	while( GetMessage( &inp->obj->msg, NULL, 0, 0 ) == 0 ){ coNext; }
			 TranslateMessage( &inp->obj->msg ); DispatchMessage( &inp->obj->msg );

    /*─······································································─*/

			if( inp->obj->msg.message == WM_MOUSEMOVE ) {
                int btX = GET_X_LPARAM( inp->obj->msg.lParam );
                int btY = GET_Y_LPARAM( inp->obj->msg.lParam );
                	inp->onMouseMotion.emit( btX, btY ); 
            }

    /*─······································································─*/

            elif( inp->obj->msg.message == WM_KEYDOWN ) { 
                     auto bt = inp->obj->msg.wParam;
                for( ulong x=inp->obj->key.size(); x--; ){
                 if( inp->obj->key[x] == bt ){ return 1; }
                }    inp->obj->key.push( bt ); 
                     inp->onKeyPress.emit( bt );
            }

            elif( inp->obj->msg.message == WM_KEYUP ) { 
                     auto bt = inp->obj->msg.wParam;
                for( ulong x=inp->obj->key.size(); x--; ){
                 if( inp->obj->key[x] == bt ) 
                   { inp->obj->key.erase(x); }
                }    inp->onKeyRelease.emit( bt ); 
            }

    /*─······································································─*/

            elif( inp->obj->msg.message == WM_LBUTTONDOWN ) { uint bt = 0;
                for( ulong x=inp->obj->key.size(); x--; ){
                 if( inp->obj->key[x] == bt ){ return 1; }
                }    inp->obj->key.push( bt ); 
                     inp->onKeyPress.emit( bt );
            }

            elif( inp->obj->msg.message == WM_LBUTTONUP ) { uint bt = 0;
                for( ulong x=inp->obj->key.size(); x--; ){
                 if( inp->obj->key[x] == bt ) 
                   { inp->obj->key.erase(x); }
                }    inp->onKeyRelease.emit( bt ); 
            }

    /*─······································································─*/

            elif( inp->obj->msg.message == WM_RBUTTONDOWN ) { uint bt = 1;
                for( ulong x=inp->obj->key.size(); x--; ){
                 if( inp->obj->key[x] == bt ){ return 1; }
                }    inp->obj->key.push( bt ); 
                     inp->onKeyPress.emit( bt );
            }

            elif( inp->obj->msg.message == WM_RBUTTONUP ) { uint bt = 1;
                for( ulong x=inp->obj->key.size(); x--; ){
                 if( inp->obj->key[x] == bt ) 
                   { inp->obj->key.erase(x); }
                }    inp->onKeyRelease.emit( bt ); 
            }

    /*─······································································─*/

            elif( inp->obj->msg.message == WM_MBUTTONDOWN ) { uint bt = 3;
                for( ulong x=inp->obj->key.size(); x--; ){
                 if( inp->obj->key[x] == bt ){ return 1; }
                }    inp->obj->key.push( bt ); 
                     inp->onKeyPress.emit( bt );
            }

            elif( inp->obj->msg.message == WM_MBUTTONUP ) { uint bt = 3;
                for( ulong x=inp->obj->key.size(); x--; ){
                 if( inp->obj->key[x] == bt ) 
                   { inp->obj->key.erase(x); }
                }    inp->onKeyRelease.emit( bt ); 
            }

    /*─······································································─*/

            if( !inp->is_closed() ) coGoto(0);
			
		coStop 
        });

    }

};}

/*────────────────────────────────────────────────────────────────────────────*/
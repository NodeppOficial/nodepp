#pragma once

#include <windows.h>

#define C_BLACK   0x00
#define C_WHITE   0x01
#define C_GREEN   0x02
#define C_RED     0x03
#define C_BLUE    0x04
#define C_CYAN    0x05
#define C_YELLOW  0x06
#define C_MAGENTA 0x07
#define C_BOLD    0x10

namespace nodepp { namespace conio { WORD attr = 0, dflt = 7;

    int background( int color ){
        if( color & 0x10 ){ attr |= BACKGROUND_INTENSITY; color &= 0x0f; }
        switch( color ) {
            case C_BLACK:   attr |= 0; return 1; break;
            case C_WHITE:   attr |= BACKGROUND_BLUE | BACKGROUND_GREEN| BACKGROUND_RED; return 1; break;
            case C_CYAN:    attr |= BACKGROUND_GREEN| BACKGROUND_BLUE; return 1; break;
            case C_YELLOW:  attr |= BACKGROUND_GREEN| BACKGROUND_RED;  break;
            case C_MAGENTA: attr |= BACKGROUND_BLUE | BACKGROUND_RED;  break;
            case C_GREEN:   attr |= BACKGROUND_GREEN; return 1; break;
            case C_RED:     attr |= BACKGROUND_RED;   return 1; break;
            case C_BLUE:    attr |= BACKGROUND_BLUE;  return 1; break;
        }   return -1;
    }

    int foreground( int color ){
        if( color & 0x10 ){ attr |= FOREGROUND_INTENSITY; color &= 0x0f; }
        switch( color ) {
            case C_BLACK:   attr |= 0; return 1; break;
            case C_WHITE:   attr |= FOREGROUND_BLUE | FOREGROUND_GREEN| FOREGROUND_RED; return 1; break;
            case C_CYAN:    attr |= FOREGROUND_GREEN| FOREGROUND_BLUE; return 1; break;
            case C_YELLOW:  attr |= FOREGROUND_GREEN| FOREGROUND_RED;  break;
            case C_MAGENTA: attr |= FOREGROUND_BLUE | FOREGROUND_RED;  break;
            case C_GREEN:   attr |= FOREGROUND_GREEN; return 1; break;
            case C_RED:     attr |= FOREGROUND_RED;   return 1; break;
            case C_BLUE:    attr |= FOREGROUND_BLUE;  return 1; break;
        }   return -1;
    }

    /*─······································································─*/

    template< class... T >
    int print( const T&... args ){ 
        if( attr != 0 ){
        SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), attr );
        } auto result = printf( args... ); attr = 0;
        SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), dflt );
        return result;
    }
    
    /*─······································································─*/

    int set_position( int x, int y ){ COORD coord; coord.X = x; coord.Y = y;
        return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    ptr_t<int> get_position(){ CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo( hConsole, &csbi );
        return {{ 
            csbi.dwCursorPosition.X,
            csbi.dwCursorPosition.Y
        }};
    }

    ptr_t<int> get_size(){ CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo( hConsole, &csbi );
        return {{
            csbi.srWindow.Right  - csbi.srWindow.Left + 1, 
            csbi.srWindow.Bottom - csbi.srWindow.Top  + 1
        }};
    }

    int inverse(){ attr |= COMMON_LVB_REVERSE_VIDEO; return 1; }

    int underscore(){ attr |= COMMON_LVB_UNDERSCORE; return 1; }

    int clear(){ return system("cls"); }

    int reset(){ attr = 0; return 1; }
    
    /*─······································································─*/

    int error( const char* msg ){ foreground( C_RED    | C_BOLD ); return print( "%s", msg ); }
    int  info( const char* msg ){ foreground( C_CYAN   | C_BOLD ); return print( "%s", msg ); }
    int  done( const char* msg ){ foreground( C_GREEN  | C_BOLD ); return print( "%s", msg ); }
    int  warn( const char* msg ){ foreground( C_YELLOW | C_BOLD ); return print( "%s", msg ); }

}}
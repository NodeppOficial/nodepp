#pragma once

#include <sys/ioctl.h>
#include <unistd.h>

#define C_BLACK   0x00
#define C_WHITE   0x01
#define C_GREEN   0x02
#define C_RED     0x03
#define C_BLUE    0x04
#define C_CYAN    0x05
#define C_YELLOW  0x06
#define C_MAGENTA 0x07
#define C_BOLD    0x10

namespace nodepp { namespace conio {

    int background( int color ){ 
        if( color & 0x10 ){ printf("\033[1m"); color &= 0x0f; }
        switch( color ) {
            case C_BLACK:   return printf("\033[40m"); break;
            case C_WHITE:   return printf("\033[47m"); break;
            case C_GREEN:   return printf("\033[42m"); break;
            case C_RED:     return printf("\033[41m"); break;
            case C_BLUE:    return printf("\033[44m"); break;
            case C_CYAN:    return printf("\033[46m"); break;
            case C_YELLOW:  return printf("\033[43m"); break;
            case C_MAGENTA: return printf("\033[45m"); break;
        }   return -1;
    }

    int foreground( int color ){
        if( color & 0x10 ){ printf("\033[1m"); color &= 0x0f; }
        switch( color ) {
            case C_BLACK:   return printf("\033[30m"); break;
            case C_WHITE:   return printf("\033[37m"); break;
            case C_GREEN:   return printf("\033[32m"); break;
            case C_RED:     return printf("\033[31m"); break;
            case C_BLUE:    return printf("\033[34m"); break;
            case C_CYAN:    return printf("\033[36m"); break;
            case C_YELLOW:  return printf("\033[33m"); break;
            case C_MAGENTA: return printf("\033[35m"); break;
        }   return -1;
    }

    /*─······································································─*/

    template< class... T >
    int print( const T&... args ){ printf( args... ); return printf("\033[0m"); }
    
    int set_position( int x, int y ){ return print("\033[%d;%dH",x,y); }

    ptr_t<int> get_size(){ struct winsize csbi;
        ioctl( STDIN_FILENO, TIOCGWINSZ, &csbi );
        return {{ csbi.ws_row, csbi.ws_col }};
    }

    int undescore(){ return printf("\033[4m"); }

    int inverse(){ return printf("\033[7m"); }

    int reset(){ return printf("\033[0m"); }
    
    int clear(){ return printf("\033c"); }

    /*─······································································─*/

    int error( const char* msg ){ foreground( C_RED    | C_BOLD ); return print( "%s", msg ); }
    int  info( const char* msg ){ foreground( C_CYAN   | C_BOLD ); return print( "%s", msg ); }
    int  done( const char* msg ){ foreground( C_GREEN  | C_BOLD ); return print( "%s", msg ); }
    int  warn( const char* msg ){ foreground( C_YELLOW | C_BOLD ); return print( "%s", msg ); }

}}
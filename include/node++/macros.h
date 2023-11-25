#ifndef NODEPP_COROUTINE
#define NODEPP_COROUTINE

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef ARDUINO
#define _Ready _init_(); int main( int argc, char** args ){ _init_(); process::pipe( argc, args ); return 0; } void _init_
#else
#define _Ready loop(){ process::next(); } void setup
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#define _Return(VALUE) do { _state_ = __LINE__; _Enable return VALUE; case __LINE__:; } while (0)
#define _Next          do { _state_ = __LINE__; _Enable return 1;     case __LINE__:; } while (0)
#define _Again         do { _state_ = __LINE__; _Enable return 0;     case __LINE__:; } while (0)
#define _Goto(VALUE)   do { _state_ = VALUE;    _Enable return 1;                     } while (0)
#define _Yield(VALUE)  do { _state_ = VALUE;    _Enable return 1;     case VALUE:;    } while (0)

/*────────────────────────────────────────────────────────────────────────────*/

#define _Start static int _state_ = 0; _Available { switch(_state_) { case 0:;
#define _End do { _state_ = 0; _Enable return -1; } while (0)
#define _Stop } _state_ = 0; _Enable return -1; }
#define _Emit int operator()

/*────────────────────────────────────────────────────────────────────────────*/

#define _Available static bool _available_ = 1; if( !_available_ ) return 1; _Disable
#define _Set(VALUE) _state_  = VALUE
#define _Get        _state_
#define _Disable _available_ = 0;
#define _Enable  _available_ = 1;

/*────────────────────────────────────────────────────────────────────────────*/

#define CHUNK_TB( VALUE ) 1024 * 1024 * 1024 * 1024 * VALUE
#define CHUNK_GB( VALUE ) 1024 * 1024 * 1024 * VALUE
#define CHUNK_MB( VALUE ) 1024 * 1024 * VALUE
#define CHUNK_KB( VALUE ) 1024 * VALUE
#define CHUNK_B( VALUE )  VALUE

/*────────────────────────────────────────────────────────────────────────────*/

#define CHUNK_SIZE 65536
#define UNBFF_SIZE 4096
#define MAX_SOCKET 4096
#define TIMEOUT    3

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class NODEPP_GENERATOR { public: NODEPP_GENERATOR() {} }; }
#define _Generator(NAME) struct NAME : public nodepp::NODEPP_GENERATOR

/*────────────────────────────────────────────────────────────────────────────*/

#define _onError( Ev, message ) if( Ev.empty() ) \
                                  { console::error(message); exit(1); } \
                                else Ev.emit( except_t( message ) );

/*────────────────────────────────────────────────────────────────────────────*/

#ifdef ARDUINO
   #define _Error( MESSAGE ) console::error ( MESSAGE );
#else
   #define _Error( MESSAGE ) throw except_t ( MESSAGE );
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#define typeof(DATA) (string_t){ typeid( DATA ).name() }

/*────────────────────────────────────────────────────────────────────────────*/

#define ullong  unsigned long long int
#define ulong   unsigned long int
#define llong   long long int
#define ldouble long double

#define ushort  unsigned short
#define uchar   unsigned char
#define uint    unsigned int
#define wchar   wchar_t

/*────────────────────────────────────────────────────────────────────────────*/
/*
#define _func_  __PRETTY_FUNCTION__
#define _name_  __FUNCTION__
#define _cpp_   __cplusplus
#define _date_  __DATE__
#define _file_  __FILE__
#define _line_  __LINE__
#define _time_  __TIME__
*/
/*────────────────────────────────────────────────────────────────────────────*/

#endif

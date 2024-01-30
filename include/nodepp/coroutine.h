#ifndef NODEPP_COROUTINE
#define NODEPP_COROUTINE

/*────────────────────────────────────────────────────────────────────────────*/

template< class T > T   min( const T& min, const T& max ){ return min < max ? min : max; }

template< class T > T   max( const T& min, const T& max ){ return max > min ? max : min; }

template< class T > T clamp( const T& val, const T& _min, const T& _max ){ return max( _min, min( _max, val ) ); }

/*────────────────────────────────────────────────────────────────────────────*/

#define _EError( Ev, message ) if ( Ev.empty() ){ console::error(message); /*exit(1);*/ } \
                               else Ev.emit( except_t( message ) );

#define _Error( MESSAGE ) throw except_t ( MESSAGE );

/*────────────────────────────────────────────────────────────────────────────*/

#define _main_ _init_(); int main( int argc, char** args ){ \
   process::start( argc, args ); _init_(); \
   process::pipe(); return 0; \
}  void _init_

/*────────────────────────────────────────────────────────────────────────────*/

#define _Return(VALUE) do { _state_ = _LINE; _Enable; return VALUE; case _LINE:; } while (0)
#define _Next          do { _state_ = _LINE; _Enable; return 1;     case _LINE:; } while (0)
#define _Again         do { _state_ = _LINE; _Enable; return 0;     case _LINE:; } while (0)
#define _Goto(VALUE)   do { _state_ = VALUE; _Enable; return 1;                  } while (0)
#define _Yield(VALUE)  do { _state_ = VALUE; _Enable; return 1;     case VALUE:; } while (0)

/*────────────────────────────────────────────────────────────────────────────*/

#define _GStart if ( !_available_ ){ return 1; } _Disable; { switch(_state_) { case 0:;
#define _GStop     }  _state_ = 0; _Enable; return -1; }

/*────────────────────────────────────────────────────────────────────────────*/

#define _Start static int _state_ = 0; _Available { switch(_state_) { case 0:;
#define _End   do { _state_ = 0; _Enable; return -1; } while (0)
#define _Stop     } _state_ = 0; _Enable; return -1; }
#define _Emit  int operator()

/*────────────────────────────────────────────────────────────────────────────*/

#define _Available static bool _available_ = 1; if( !_available_ ){ return 1; } _Disable;
#define _Generator(NAME) struct NAME : public NODEPP_GENERATOR_BASE
#define _Disable         _available_ = 0
#define _Enable          _available_ = 1
#define _Set(VALUE)      _state_ = VALUE
#define _Get             _state_

/*────────────────────────────────────────────────────────────────────────────*/

#define _FUNC  __PRETTY_FUNCTION__
#define _NAME  __FUNCTION__
#define _DATE  __DATE__
#define _FILE  __FILE__
#define _LINE  __LINE__
#define _TIME  __TIME__

/*────────────────────────────────────────────────────────────────────────────*/

#define CHUNK_TB( VALUE ) ( 1024 * 1024 * 1024 * 1024 * VALUE )
#define CHUNK_GB( VALUE ) ( 1024 * 1024 * 1024 * VALUE )
#define CHUNK_MB( VALUE ) ( 1024 * 1024 * VALUE )
#define CHUNK_KB( VALUE ) ( 1024 * VALUE )
#define CHUNK_B( VALUE )  ( VALUE )

/*────────────────────────────────────────────────────────────────────────────*/

#define MAX_SOCKET SOMAXCONN
#define CHUNK_SIZE 65536
#define SSL_SIZE   16384
#define UNBFF_SIZE 4096
#define TIMEOUT    3

/*────────────────────────────────────────────────────────────────────────────*/

#define typeof(DATA) (string_t){ typeid( DATA ).name() }

struct NODEPP_GENERATOR_BASE { protected:
   bool _available_ = 1;
   int  _state_     = 0;
};

#define ullong  unsigned long long int
#define ulong   unsigned long int
#define llong   long long int
#define ldouble long double

#define ushort  unsigned short
#define uchar   unsigned char
#define uint    unsigned int
#define wchar   wchar_t

/*────────────────────────────────────────────────────────────────────────────*/

#define forEach( ITEM, CB ) for( auto& x : ITEM ){ CB( x ); }

#define forEver() for (;;)

#define elif else if

/*────────────────────────────────────────────────────────────────────────────*/

#define NODEPP_KERNEL_WINDOWS 4
#define NODEPP_KERNEL_POSIX   3
#define NODEPP_KERNEL_ARDUINO 2
#define NODEPP_KERNEL_WASM    1
#define NODEPP_KERNEL_UNKNOWN 0

#ifndef    _KERNEL
#if defined(WINDOWS) || defined(_WIN32) || defined(_WIN64)
   #define _KERNEL NODEPP_KERNEL_WINDOWS
#elif defined(__EMSCRIPTEN__)
   #define _ENVIRONMENT NODEPP_ENVIRONMENT_WASM
#elif defined(ARDUINO)
   #define _KERNEL NODEPP_KERNEL_ARDUINO
#elif defined(__linux__)
   #define _KERNEL NODEPP_KERNEL_POSIX
#else
   #define _KERNEL NODEPP_KERNEL_UNKNOWN
#endif
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#define NODEPP_OS_WINDOWS 8
#define NODEPP_OS_ANDROID 7
#define NODEPP_OS_IOS     6
#define NODEPP_OS_TIZEN   5
#define NODEPP_OS_APPLE   4
#define NODEPP_OS_FRBSD   3
#define NODEPP_OS_LINUX   2
#define NODEPP_OS_ARDUINO 1
#define NODEPP_OS_UNKNOWN 0

#ifndef    _OS
#if defined(WINDOWS) || defined(_WIN32) || defined(_WIN64)
   #define _OS NODEPP_OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
   #define _OS NODEPP_OS_IOS
#elif defined(__TIZEN__)
   #define _OS NODEPP_OS_TIZEN
#elif defined(__ANDROID__)
   #define _OS NODEPP_OS_ANDROID
#elif defined(__FreeBSD__)
   #define _OS NODEPP_OS_FRBSD
#elif defined(__APPLE__)
   #define _OS NODEPP_OS_APPLE
#elif defined(__linux__)
   #define _OS NODEPP_OS_LINUX
#elif defined(ARDUINO)
   #define _OS NODEPP_OS_ARDUINO
#else
   #define _OS NODEPP_OS_UNKNOWN
#endif
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#define NODEPP_ARCH_CPU_64  4
#define NODEPP_ARCH_CPU_32  3
#define NODEPP_ARCH_ARM_64  2
#define NODEPP_ARCH_ARM_32  1
#define NODEPP_ARCH_UNKNOWN 0

#ifndef       _ARCH
#if defined(__GNUC__)
   #if defined(__x86_64__)
      #define _ARCH NODEPP_ARCH_CPU_64
   #elif defined(__aarch64__)
      #define _ARCH NODEPP_ARCH_ARM_64
   #elif defined(__i386__)
      #define _ARCH NODEPP_ARCH_CPU_32
   #elif defined(__arm__)
      #define _ARCH NODEPP_ARCH_ARM_64
   #else
      #define _ARCH NODEPP_ARCH_UNKNOWN
   #endif
#else
   #if defined(_M_IX86)
      #define _ARCH NODEPP_ARCH_CPU_32
   #elif defined(_M_ARM64)
      #define _ARCH NODEPP_ARCH_ARM_64
   #elif defined(_M_X64)
      #define _ARCH NODEPP_ARCH_CPU_64
   #elif defined(_M_ARM)
      #define _ARCH NODEPP_ARCH_ARM_32
   #else
      #define _ARCH NODEPP_ARCH_UNKNOWN
   #endif
#endif
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#define NODEPP_ENVIRONMENT_GNU     5
#define NODEPP_ENVIRONMENT_MSYS2   4
#define NODEPP_ENVIRONMENT_MINGW   3
#define NODEPP_ENVIRONMENT_CYWIN   2
#define NODEPP_ENVIRONMENT_UNKNOWN 0

#ifndef    _ENVIRONMENT
#if defined(__MSYS__)
   #define _ENVIRONMENT NODEPP_ENVIRONMENT_MSYS2
#elif defined(__CYGWIN__)
   #define _ENVIRONMENT NODEPP_ENVIRONMENT_CYWIN
/*
#elif defined(__MINGW32__) || defined(__MINGW64__)
   #define _ENVIRONMENT NODEPP_ENVIRONMENT_MINGW
#elif defined(__GNUC__)
   #define _ENVIRONMENT NODEPP_ENVIRONMENT_GNU
*/
#else
   #define _ENVIRONMENT NODEPP_ENVIRONMENT_UNKNOWN
#endif
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#define NODEPP_POLL_WPOLL  3
#define NODEPP_POLL_KPOLL  2
#define NODEPP_POLL_EPOLL  1
#define NODEPP_POLL_POLL   0

#ifndef    _POLL
#if   _OS == NODEPP_OS_WINDOWS
   #define _POLL NODEPP_POLL_WPOLL
#elif _OS == NODEPP_OS_APPLE
   #define _POLL NODEPP_POLL_KPOLL
#elif _OS == NODEPP_OS_FRBSD
   #define _POLL NODEPP_POLL_KPOLL
#elif _OS == NODEPP_OS_LINUX
   #define _POLL NODEPP_POLL_EPOLL
#else
   #define _POLL NODEPP_POLL
#endif
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif

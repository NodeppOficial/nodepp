#ifndef NODEPP_COROUTINE
#define NODEPP_COROUTINE

/*────────────────────────────────────────────────────────────────────────────*/

template< class T > T   min( const T& min, const T& max ){ return min < max ? min : max; }

template< class T > T   max( const T& min, const T& max ){ return max > min ? max : min; }

template< class T > T clamp( const T& val, const T& _min, const T& _max ){ return max( _min, min( _max, val ) ); }

/*────────────────────────────────────────────────────────────────────────────*/

#define $Ready $Init(); int main( int argc, char** args ){ \
   process::start( argc, args ); $Init(); \
   process::pipe(); return 0; \
}  void $Init

#define $Main $Ready

/*────────────────────────────────────────────────────────────────────────────*/

#define $Return(VALUE) do { _state_ = $LINE; $Enable; return VALUE; case $LINE:; } while (0)
#define $Next          do { _state_ = $LINE; $Enable; return 1;     case $LINE:; } while (0)
#define $Again         do { _state_ = $LINE; $Enable; return 0;     case $LINE:; } while (0)
#define $Goto(VALUE)   do { _state_ = VALUE; $Enable; return 1;                  } while (0)
#define $Yield(VALUE)  do { _state_ = VALUE; $Enable; return 1;     case VALUE:; } while (0)

/*────────────────────────────────────────────────────────────────────────────*/

#define $Start static int _state_ = 0; $Available { switch(_state_) { case 0:;
#define $End   do { _state_ = 0; $Enable; return -1; } while (0)
#define $Stop     } _state_ = 0; $Enable; return -1; }
#define $Emit  int operator()

/*────────────────────────────────────────────────────────────────────────────*/

#define $Available static bool _available_ = 1; if( !_available_ ) return 1; $Disable;
#define $Disable     _available_ = 0
#define $Enable      _available_ = 1
#define $Set(VALUE)  _state_ = VALUE
#define $Get         _state_

/*────────────────────────────────────────────────────────────────────────────*/

#define CHUNK_TB( VALUE ) ( 1024 * 1024 * 1024 * 1024 * VALUE )
#define CHUNK_GB( VALUE ) ( 1024 * 1024 * 1024 * VALUE )
#define CHUNK_MB( VALUE ) ( 1024 * 1024 * VALUE )
#define CHUNK_KB( VALUE ) ( 1024 * VALUE )
#define CHUNK_B( VALUE )  ( VALUE )

/*────────────────────────────────────────────────────────────────────────────*/

#define MAX_SOCKET SOMAXCONN
#define CHUNK_SIZE 65536
#define UNBFF_SIZE 4096
#define TIMEOUT    1

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class NODEPP_GENERATOR { public: NODEPP_GENERATOR() {} }; }
#define $Generator(NAME) struct NAME : public nodepp::NODEPP_GENERATOR

/*────────────────────────────────────────────────────────────────────────────*/

#define $onError( Ev, message ) if( Ev.empty() ) \
                                  { console::error(message); exit(1); } \
                                else Ev.emit( except_t( message ) );

/*────────────────────────────────────────────────────────────────────────────*/

#define $Error( MESSAGE ) throw except_t ( MESSAGE );

/*────────────────────────────────────────────────────────────────────────────*/

#define typeof(DATA) (string_t){ typeid( DATA ).name() }

#define ullong  unsigned long long int
#define ulong   unsigned long int
#define llong   long long int
#define ldouble long double

#define ushort  unsigned short
#define uchar   unsigned char
#define uint    unsigned int
#define wchar   wchar_t

/*────────────────────────────────────────────────────────────────────────────*/

#define $FUNC  __PRETTY_FUNCTION__
#define $NAME  __FUNCTION__
#define $DATE  __DATE__
#define $FILE  __FILE__
#define $LINE  __LINE__
#define $TIME  __TIME__

/*────────────────────────────────────────────────────────────────────────────*/

#define NODEPP_KERNEL_WINDOWS 3
#define NODEPP_KERNEL_POSIX   2
#define NODEPP_KERNEL_ARDUINO 1
#define NODEPP_KERNEL_UNKNOWN 0

#ifndef $KERNEL
#if defined(WINDOWS) || defined(_WIN32) || defined(_WIN64)
   #define $KERNEL NODEPP_KERNEL_WINDOWS
#elif defined(__linux__)
   #define $KERNEL NODEPP_KERNEL_POSIX
#elif defined(ARDUINO)
   #define $KERNEL NODEPP_KERNEL_ARDUINO
#else
   #define $KERNEL NODEPP_KERNEL_UNKNOWN
#endif
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#define NODEPP_OS_WINDOWS 7
#define NODEPP_OS_ANDROID 6
#define NODEPP_OS_IOS     5
#define NODEPP_OS_TIZEN   4
#define NODEPP_OS_APPLE   3
#define NODEPP_OS_LINUX   2
#define NODEPP_OS_ARDUINO 1
#define NODEPP_OS_UNKNOWN 0

#ifndef $OS
#if defined(WINDOWS) || defined(_WIN32) || defined(_WIN64)
   #define $OS NODEPP_OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
   #define $OS NODEPP_OS_IOS
#elif defined(__TIZEN__)
   #define $OS NODEPP_OS_TIZEN
#elif defined(__ANDROID__)
   #define $OS NODEPP_OS_ANDROID
#elif defined(__APPLE__)
   #define $OS NODEPP_OS_APPLE
#elif defined(__linux__)
   #define $OS NODEPP_OS_LINUX
#elif defined(ARDUINO)
   #define $OS NODEPP_OS_ARDUINO
#else
   #define $OS NODEPP_OS_UNKNOWN
#endif
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#define NODEPP_ARCH_CPU_64  4
#define NODEPP_ARCH_CPU_32  3
#define NODEPP_ARCH_ARM_64  2
#define NODEPP_ARCH_ARM_32  1
#define NODEPP_ARCH_UNKNOWN 0

#ifndef $ARCH
#if defined(__GNUC__)
   #if defined(__x86_64__)
      #define $ARCH NODEPP_ARCH_CPU_64
   #elif defined(__aarch64__)
      #define $ARCH NODEPP_ARCH_ARM_64
   #elif defined(__i386__)
      #define $ARCH NODEPP_ARCH_CPU_32
   #elif defined(__arm__)
      #define $ARCH NODEPP_ARCH_ARM_64
   #else
      #define $ARCH NODEPP_ARCH_UNKNOWN
   #endif
#else
   #if defined(_M_IX86)
      #define $ARCH NODEPP_ARCH_CPU_32
   #elif defined(_M_ARM64)
      #define $ARCH NODEPP_ARCH_ARM_64
   #elif defined(_M_X64)
      #define $ARCH NODEPP_ARCH_CPU_64
   #elif defined(_M_ARM)
      #define $ARCH NODEPP_ARCH_ARM_32
   #else
      #define $ARCH NODEPP_ARCH_UNKNOWN
   #endif
#endif
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#define NODEPP_ENVIRONMENT_GNU     4
#define NODEPP_ENVIRONMENT_MSYS2   3
#define NODEPP_ENVIRONMENT_MINGW   2
#define NODEPP_ENVIRONMENT_CYWIN   1
#define NODEPP_ENVIRONMENT_UNKNOWN 0

#ifndef $ENVIRONMENT
#if defined(__MSYS__)
   #define $ENVIRONMENT NODEPP_ENVIRONMENT_MSYS2
#elif defined(__CYGWIN__)
   #define $ENVIRONMENT NODEPP_ENVIRONMENT_CYWIN
/*
#elif defined(__MINGW32__) || defined(__MINGW64__)
   #define $ENVIRONMENT NODEPP_ENVIRONMENT_MINGW
#elif defined(__GNUC__)
   #define $ENVIRONMENT NODEPP_ENVIRONMENT_GNU
*/
#else
   #define $ENVIRONMENT NODEPP_ENVIRONMENT_UNKNOWN
#endif
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#endif

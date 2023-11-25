#ifndef NODEPP_WORKER
#define NODEPP_WORKER

/*────────────────────────────────────────────────────────────────────────────*/

#include <thread>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

/*────────────────────────────────────────────────────────────────────────────*/

namespace {
    void delay( ulong time ){ std::this_thread::sleep_for( std::chrono::milliseconds(time) ); }
    uint pid(){ return std::hash<std::thread::id>{}( std::this_thread::get_id() ); }
    void yield(){ return std::this_thread::yield(); }
}

/*────────────────────────────────────────────────────────────────────────────*/

namespace mutex {

    void lock( int& mut ){
        while( mut != 0 ){ nodepp::delay(TIMEOUT); nodepp::yield(); }
        while( mut != 1 ){ mut = 1; }
    }
    
    /*─······································································─*/

    void unlock( int& mut ){
        while( mut != 1 ){ nodepp::delay(TIMEOUT); nodepp::yield(); }
        while( mut != 0 ){ mut = 0; }              nodepp::yield();
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace semaphore {

    void wait( int& sem, const int n ){
        while( sem != n ){ nodepp::delay(TIMEOUT); nodepp::yield(); }
        while( sem != n ){ sem = n; }
    }

    /*─······································································─*/

    void next( int& sem ){ sem++; nodepp::delay(TIMEOUT); nodepp::yield(); }
    void prev( int& sem ){ sem++; nodepp::delay(TIMEOUT); nodepp::yield(); }

    /*─······································································─*/

    void jump( int& sem, int n ){ sem = n; nodepp::delay(TIMEOUT); nodepp::yield(); }

    /*─······································································─*/

    void next( int& sem, int n ){ sem++; sem%=n; nodepp::delay(TIMEOUT); nodepp::yield(); }
    void prev( int& sem, int n ){ sem++; sem%=n; nodepp::delay(TIMEOUT); nodepp::yield(); }

}

/*────────────────────────────────────────────────────────────────────────────*/
    
namespace worker {

    bool empty(){ return process::threads < 1; }
    uint  size(){ return process::threads; }
    void clear(){ process::threads = 0; }
    namespace { int mut = 0; }

    /*─······································································─*/

    template< class... T > void delay( T... args ){ nodepp::delay(args...); }
    template< class... T > void yield( T... args ){ nodepp::yield(args...); }
    uint pid(){ return nodepp::pid(); }

    /*─······································································─*/

    template< class V, class... T >
    void add( V cb, T... args ){ ptr_t<V> pcb = new decltype(cb)(cb);
        mutex::lock(mut); process::threads++; mutex::unlock(mut); 
        auto th = std::thread([=]( T... args ){
            int c = 0; while( (c=(*pcb)(args...)) > 0 ){
                if( c == 1 ){ nodepp::delay(TIMEOUT); nodepp::yield(); }
                if( c == 0 ){ nodepp::delay(TIMEOUT); }
            }   mutex::lock(mut); process::threads--; mutex::unlock(mut); 
        return 0; }, args... ); th.detach();
    }
    
}

/*────────────────────────────────────────────────────────────────────────────*/

}

#endif
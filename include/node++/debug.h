#ifndef NODEPP_DEBUG
#define NODEPP_DEBUG

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class debug_t {     
protected: 

    string_t message; ulong ev;

public:

    virtual ~debug_t() noexcept { 
	    console::log( message, "closed" );  
    #ifdef NODEPP_SIGNAL
   	    process::onSIGERR.off(ev);
    #endif
    }
    
    /*─······································································─*/

    debug_t( const string_t& msg ) noexcept {
    #ifdef NODEPP_SIGNAL
        ev = process::onSIGERR([=]( int sig ){ error(); });
    #endif
	    console::log( msg, "open" ); message = msg; 
    }
    
    /*─······································································─*/

    void log( const string_t& msg ) const noexcept { console::log( "--",  msg ); }

    void error() const noexcept { console::error( message ); }
    
    /*─······································································─*/

    debug_t( const debug_t& o) noexcept = delete;
    debug_t& operator=( const debug_t& o ) noexcept = delete;

    debug_t( const debug_t&& o) noexcept = delete;
    debug_t& operator=( debug_t&& o ) noexcept = delete;

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_DEBUG
#define NODEPP_DEBUG

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class debug_t {     
protected: 

    string_t message; ulong ev;

public:

    virtual ~debug_t() noexcept { 
	    console::log( message, "closed" );  
   	    process::onSIGERR.off(ev);
    }
    
    /*─······································································─*/

    debug_t( const string_t& msg ) noexcept {
        ev = process::onSIGERR([=]( int sig ){ error(); });
	    console::log( msg, "open" ); message = msg; 
    }
    
    /*─······································································─*/

    debug_t() noexcept: message("something went wrong") {
        ev = process::onSIGERR([=]( int sig ){ error(); });
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
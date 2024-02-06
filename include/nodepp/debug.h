#ifndef NODEPP_DEBUG
#define NODEPP_DEBUG

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class debug_t {     
protected: 

    string_t message; void* ev = nullptr;

public:

    virtual ~debug_t() noexcept { 
	    console::log( message, "closed" );  
   	    process::onSIGERR.off(ev);
    }
    
    /*─······································································─*/

    debug_t( const string_t& msg ) noexcept {
        auto inp = type::bind( this );
        ev = process::onSIGERR([=](){ inp->error(); });
	    console::log( msg, "open" ); message = msg; 
    }
    
    debug_t() noexcept: message("something went wrong") {
        auto inp = type::bind( this );
        ev = process::onSIGERR.once([=](){ inp->error(); });
    }
    
    /*─······································································─*/

    template< class... T >
    void log( const T&... args ) const noexcept { console::log( "--", args... ); }

    void error() const noexcept { console::error( message ); }
    
    /*─······································································─*/

    debug_t( const debug_t& o) noexcept = delete;
    debug_t& operator=( const debug_t& o ) noexcept = delete;

    debug_t( const debug_t&& o) noexcept = delete;
    debug_t& operator=( debug_t&& o ) noexcept = delete;

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
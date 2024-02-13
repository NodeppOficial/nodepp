#ifndef NODEPP_DEBUG
#define NODEPP_DEBUG

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class debug_t {     
protected: 

    struct _str_ { 
        void * ev = nullptr;
        string_t msg;
    };  ptr_t<_str_> obj;

public:

    virtual ~debug_t() noexcept { 
    //  if ( obj.count() > 2 ){ return; }
	    console::log( obj->msg, "closed" );  
   	    process::onSIGERR.off(obj->ev);
    }
    
    /*─······································································─*/

    debug_t( const string_t& msg ) noexcept : obj(new _str_()) {
        obj->msg = msg; 
        auto inp = type::bind( this );
        obj->ev  = process::onSIGERR([=](){ inp->error(); });
	               console::log( obj->msg, "open" );
    }
    
    debug_t() noexcept : obj(new _str_()) {
        auto inp = type::bind( this );
        obj->msg = "something went wrong";
        obj->ev  = process::onSIGERR([=](){ inp->error(); });
	               console::log( obj->msg, "open" );
    }
    
    /*─······································································─*/

    template< class... T >
    void log( const T&... args ) const noexcept { console::log( "--", args... ); }

    void error() const noexcept { console::error( obj->msg ); }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
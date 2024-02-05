#ifndef NODEPP_EVENT
#define NODEPP_EVENT

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class... A > class event_t { 
protected:

    struct _str_ {
        function_t<void,A...> cb;
        bool                  on;
    };  using ev = queue_t<_str_>;

        ptr_t<ev> obj;

public: event_t() noexcept : obj( new ev ) {}
    
    /*─······································································─*/

    void* operator()( decltype(_str_::cb) func ) const noexcept { return on(func); }
    
    /*─······································································─*/

    ulong empty() const noexcept { return obj->empty(); }
    ulong  size() const noexcept { return obj->size(); }
    void  clear() const noexcept { obj->clear(); }
    
    /*─······································································─*/

    void emit( const A&... args ) const noexcept {
        auto x = obj->first(); while( x != nullptr ){
            x->data.cb( args... ); if ( !x->data.on )
               { auto y = x->next(); obj->erase(x); x=y; continue; } 
            else x = x->next(); 
        }
    }
    
    /*─······································································─*/

    void off( void* id ) const noexcept {
        auto x = obj->first(); while( x != nullptr ){
            if ( (void*) &x->data == id )
               { auto y = x->next(); obj->erase(x); x=y; continue; } 
            else x = x->next(); 
        }
    }

    void* once( function_t<void,A...> func ) const noexcept {
        obj->push({ func, 0 }); 
        return (void*) obj->last();
    }

    void* on( function_t<void,A...> func ) const noexcept {
        obj->push({ func, 1 }); 
        return (void*) obj->last();
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

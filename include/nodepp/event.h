#ifndef NODEPP_EVENT
#define NODEPP_EVENT

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class... A > class event_t { 
protected:

    struct NODE {
        function_t<void,A...> cb;
        bool                  on;
    };  using ev = queue_t<NODE>;

        ptr_t<ev> obj;

public: event_t() noexcept : obj( new ev ) {}
    
    /*─······································································─*/

    void* operator()( function_t<void,A...> func ) const noexcept { return on(func); }
    
    /*─······································································─*/

    ulong empty() const noexcept { return obj->empty(); }
    ulong  size() const noexcept { return obj->size(); }
    void  clear() const noexcept { obj->clear(); }
    
    /*─······································································─*/

    void emit( const A&... args ) const noexcept {
        auto x = obj->first(); while( x != nullptr ){
             x->data.cb( args... ); if( !x->data.on )
            { auto y=x->next; obj->erase(x); x=y; } 
              else x=x->next; 
        }
    }
    
    /*─······································································─*/

    void off( void* id ) const noexcept { auto x = obj->first(); 
        while( id != nullptr && x != nullptr ){
            if ( x == id ){ obj->erase(x); break; } 
            else x = x->next; 
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
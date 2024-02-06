#ifndef NODEPP_EVENT
#define NODEPP_EVENT

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class... A > class event_t { 
protected:

    using ob = function_t<void,A...>;
    using ev = queue_t<ob>;

    ptr_t<ev> once_queue, every_queue;

public: event_t() noexcept : once_queue(new ev), every_queue(new ev) {}
    
    /*─······································································─*/

    void* operator()( function_t<void,A...> func ) const noexcept { return on(func); }
    
    /*─······································································─*/

    ulong empty() const noexcept { return ( every_queue->empty() && once_queue->empty() ); }
    ulong  size() const noexcept { return once_queue->size() + every_queue->size(); }
    void  clear() const noexcept { every_queue->clear(); once_queue->clear(); }
    
    /*─······································································─*/

    void emit( const A&... args ) const noexcept {
        every_queue->map([=]( ob arg ){ arg(args...); });
         once_queue->map([=]( ob arg ){ arg(args...); });
        if( !once_queue->empty() ) once_queue->clear();
    }
    
    /*─······································································─*/

    void off( void* id ) const noexcept {
        ulong index_A = every_queue->index_of([=]( ob di ){ return &di == id; });
        ulong index_B =  once_queue->index_of([=]( ob di ){ return &di == id; });
        every_queue->erase( every_queue->get( index_A ) ); 
         once_queue->erase(  once_queue->get( index_B ) );
    }

    void* once( function_t<void,A...> func ) const noexcept {
                       once_queue->push( func ); 
        return (void*) once_queue->last();
    }

    void* on( function_t<void,A...> func ) const noexcept {
                       every_queue->push( func );
        return (void*) every_queue->last();
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
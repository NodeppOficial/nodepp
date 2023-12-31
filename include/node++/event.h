#ifndef NODEPP_EVENT
#define NODEPP_EVENT

/*────────────────────────────────────────────────────────────────────────────*/

#include "hash.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class... A > class event_t { 
protected:

    struct _str_ {
        ulong                 id;
        function_t<void,A...> cb;
    };  using ev = queue_t<_str_>;
    
    ptr_t<ev> once_queue, every_queue;

public: event_t() noexcept : once_queue(new ev), every_queue(new ev) {}
    
    /*─······································································─*/

    void emit( const A&... args ) const noexcept {
        every_queue->map([=]( _str_ arg ){ arg.cb(args...); });
        once_queue->map([=]( _str_ arg ) { arg.cb(args...); });
        if( !once_queue->empty() ) once_queue->clear();
    }
    
    /*─······································································─*/

    ulong empty() const noexcept { return ( every_queue->empty() && once_queue->empty() ); }
    ulong operator()( function_t<void,A...> func ) const noexcept { return on(func); }
    ulong  size() const noexcept { return once_queue->size() + every_queue->size(); }
    void  clear() const noexcept { every_queue->clear(); once_queue->clear(); }
    
    /*─······································································─*/

    void off( ulong _hash ) const noexcept {
        ulong index_A = every_queue->index_of([=]( _str_ data ){ return data.id == _hash; });
        ulong index_B = once_queue->index_of([=]( _str_ data ){ return data.id == _hash; });
        every_queue->erase( every_queue->get( index_A ) ); once_queue->erase( once_queue->get( index_B ) );
    }

    ulong once( function_t<void,A...> func ) const noexcept {
        ulong _hash = nodepp::hash::hash();
        once_queue->push({ _hash, func }); 
        return _hash;
    }

    ulong on( function_t<void,A...> func ) const noexcept {
        ulong _hash = nodepp::hash::hash();
        every_queue->push({ _hash, func });
        return _hash;
    }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

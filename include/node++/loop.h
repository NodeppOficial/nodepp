#ifndef NODEPP_LOOP
#define NODEPP_LOOP

namespace nodepp { namespace process {

/*────────────────────────────────────────────────────────────────────────────*/

namespace task {

    queue_t<function_t<int>> queue;

    bool empty(){ return queue.empty(); }

    ulong size(){ return queue.size(); }

    void clear(){ queue.clear(); }

    template< class T, class... V >
    void add( T cb, V... arg ){ 
        ptr_t<T> pcb = new decltype(cb)(cb); 
        queue.push([=](){ return (*pcb)(arg...); });
    }

    void next(){
        if( queue.empty() ){ return; }
        int result = queue.get()->data();
             if( result == 1 ){ queue.next(); }
        else if( result <  0 ){ queue.erase( queue.get() ); }
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace loop {

    queue_t<function_t<int>> queue;

    bool empty(){ return queue.empty(); }

    ulong size(){ return queue.size(); }

    void clear(){ queue.clear(); }

    template< class T, class... V >
    void add( T cb, V... arg ){ 
        ptr_t<T> pcb = new decltype(cb)(cb); 
        queue.push([=](){ return (*pcb)(arg...); });
    }

    void next(){
        if( queue.empty() ){ return; }
        int result = queue.get()->data();
             if( result == 1 ){ queue.next(); }
        else if( result <  0 ){ queue.erase( queue.get() ); }
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace poll {

    queue_t<function_t<int>> queue;

    bool empty(){ return queue.empty(); }

    ulong size(){ return queue.size(); }

    void clear(){ queue.clear(); }

    template< class T, class... V >
    void add( T cb, V... arg ){ 
        ptr_t<T> pcb = new decltype(cb)(cb); 
        queue.push([=](){ return (*pcb)(arg...); });
    }

    void next(){ 
        if( queue.empty() ){ return; }
        int result = queue.get()->data();
             if( result == 1 ){ queue.next(); }
        else if( result <  0 ){ queue.erase( queue.get() ); }
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

}}

#endif
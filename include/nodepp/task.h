#ifndef NODEPP_TASK
#define NODEPP_TASK

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process { event_t<> onNext;

namespace task {

    queue_t<function_t<int>> queue;

    bool empty(){ return queue.empty(); }

    ulong size(){ return queue.size(); }

    void clear(){ queue.clear(); }

    template< class T, class... V >
    void add( T cb, const V&... arg ){ 
        ptr_t<type::pair<bool,T>> pb = new type::pair<bool,T>({ 0, cb });
        queue.push([=](){ 
            if(pb->first){ return 1; } pb->first = 1;
            int rs = (pb->second)(arg...);
            pb->first = 0; return rs; 
        });
    }

    void next(){ onNext.emit();
        if( queue.empty() ){ return; }
          auto x = queue.get();
          int  y = x->data();
          if ( y == 1 ){ queue.next(); }
        elif ( y <  0 ){ queue.erase( x ); }
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace loop {

    queue_t<function_t<int>> queue;

    bool empty(){ return queue.empty(); }

    ulong size(){ return queue.size(); }

    void clear(){ queue.clear(); }

    template< class T, class... V >
    void add( T cb, const V&... arg ){ 
        ptr_t<type::pair<bool,T>> pb = new type::pair<bool,T>({ 0, cb });
        queue.push([=](){ 
            if(pb->first){ return 1; } pb->first = 1;
            int rs = (pb->second)(arg...);
            pb->first = 0; return rs; 
        });
    }

    void next(){ onNext.emit();
        if( queue.empty() ){ return; }
          auto x = queue.get();
          int  y = x->data();
          if ( y == 1 ){ queue.next(); }
        elif ( y <  0 ){ queue.erase( x ); }
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace poll {

    queue_t<function_t<int>> queue;

    bool empty(){ return queue.empty(); }

    ulong size(){ return queue.size(); }

    void clear(){ queue.clear(); }

    template< class T, class... V >
    void add( T cb, const V&... arg ){ 
        ptr_t<type::pair<bool,T>> pb = new type::pair<bool,T>({ 0, cb });
        queue.push([=](){ 
            if(pb->first){ return 1; } pb->first = 1;
            int rs = (pb->second)(arg...);
            pb->first = 0; return rs; 
        });
    }

    void next(){ onNext.emit();
        if( queue.empty() ){ return; }
          auto x = queue.get();
          int  y = x->data();
          if ( y == 1 ){ queue.next(); }
        elif ( y <  0 ){ queue.erase( x ); }
    }

}

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
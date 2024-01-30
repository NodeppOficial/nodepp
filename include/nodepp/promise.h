#ifndef NODE_PROMISE
#define NODE_PROMISE

/*────────────────────────────────────────────────────────────────────────────*/

#include "tuple.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace promise {

    template< class T, class V > void resolve( 
        function_t<void,function_t<void,T>,function_t<void,V>> func,
        function_t<void,T> res, function_t<void,V> rej
    ){  process::task::add([=](){ func(
            [=]( T data ){ res(data); },
            [=]( V data ){ rej(data); }
        ); return -1; });
    }

    /*─······································································─*/

    template< class T > void resolve( 
        function_t<void,function_t<void,T>> func,
        function_t<void,T> res
    ){  process::task::add([=](){ func(
            [=]( T data ){ res(data); }
        ); return -1; });
    }

    /*─······································································─*/

    template< class T > T await ( 
        function_t<void,function_t<void,T>> func 
    ){  T result; ptr_t<int> done = new int(0); 
        resolve<T>( func, [&]( T res ){ result = res; *done = 1; });
        while( *done == 0 ){ process::next(); } return result;
    }

    /*─······································································─*/

    template< class T, class V > tuple_t<int,T,V> await ( 
        function_t<void,function_t<void,T>,function_t<void,V>> func 
    ){  T res; V rej; int st=-1; ptr_t<int> done = new int(0); 
        promise::resolve<T,V>( func, 
            [&]( T _data ){ res = _data; st=0; *done = 1; }, 
            [&]( V _data ){ rej = _data; st=1; *done = 1; }
        );
        while( *done == 0 ){ process::next(); } 
        return tuple_t<int,T,V> (st,res,rej);
    }
    
    /*─······································································─*/

    void clear( ptr_t<int>& address ){ if( !address ) *address = 0; }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class T, class V > class promise_t { 
protected:

    struct _str_ {
        function_t<void,function_t<void,T>,function_t<void,V>> main_func;
        function_t<void,T> res_func;
        function_t<void,V> rej_func;
        function_t<void>   fin_func; int state = 0;
    };  ptr_t<_str_> obj = new _str_();

public:

    promise_t& then( decltype(obj->res_func) cb ) noexcept { obj->state=2; obj->res_func = cb; return (*this); }
    
    promise_t& fail( decltype(obj->rej_func) cb ) noexcept { obj->state=2; obj->rej_func = cb; return (*this); }

    /*─······································································─*/

    promise_t( decltype(obj->main_func) cb ) noexcept { obj->main_func = cb; }

    virtual ~promise_t() noexcept {
        if( obj.count()>1 || obj->state != 2 ){ return; } obj->state=0;
        promise::resolve<T,V>( obj->main_func, obj->res_func, obj->rej_func ); 
    }

    /*─······································································─*/

    void resolve() const noexcept { if( obj->state==2 ){ return; } obj->state=0;
        promise::resolve<T,V>( obj->main_func, obj->res_func, obj->rej_func ); 
    }

    /*─······································································─*/

    tuple_t<int,T,V> await() const noexcept { T res; V rej;
        if( obj->state==2 ) return tuple_t<int,T,V>( 1, res, rej );
            obj->state =0;  return promise::await<T,V>( obj->main_func );
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
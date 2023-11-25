#ifndef NODEPP_FUNCTION
#define NODEPP_FUNCTION

/*────────────────────────────────────────────────────────────────────────────*/

#include "buffer.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class V, class... T > class function_t {
public:
    
   ~function_t() {}

    function_t() : func_ptr(nullptr) {}
    
    template< class F >
    function_t( F f ) : func_ptr( new func_impl<F>(f) ) {}
    
    /*─······································································─*/
    
    V operator()( T... arg ) const { return func_ptr->invoke(arg...); }
    
private:

    class func_base { public:
        virtual ~func_base() {}
        virtual V invoke( T... arg ) const = 0;
    };
    
    /*─······································································─*/
    
    template< class F >
    class func_impl : public func_base {

    public:

        func_impl( F f ) : func(f) {}
        virtual V invoke( T... arg ) const { return func(arg...); }

    private:
        F func;
    };
    
    /*─······································································─*/
    
    ptr_t<func_base> func_ptr;
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
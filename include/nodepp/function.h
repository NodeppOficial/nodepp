#ifndef NODEPP_FUNCTION
#define NODEPP_FUNCTION

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { template< class V, class... T > class function_t {
public:
    
    template< class F >
    function_t( F f ) : func_ptr( new func_impl<F>(f) ) {}
   
    function_t() noexcept : func_ptr(nullptr) {}
    
    virtual ~function_t() noexcept = default;
    
    /*─······································································─*/

    ulong count() const noexcept { return func_ptr.count(); }

    bool empty()  const noexcept { return func_ptr.null(); }
    
    /*─······································································─*/
    
    V operator()( const T&... arg ) const { return func_ptr->invoke(arg...); }

    explicit operator bool(void) const noexcept { return func_ptr.null(); }
    
private:

    class func_base { public:
        virtual ~func_base() {}
        virtual V invoke( const T&... arg ) const = 0;
    };
    
    /*─······································································─*/
    
    template< class F >
    class func_impl : public func_base {

    public:

        func_impl( const F& f ) : func(f) {}
        virtual V invoke( const T&... arg ) const { return func(arg...); }

    private:
        F func;
    };
    
    /*─······································································─*/
    
    ptr_t<func_base> func_ptr;
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
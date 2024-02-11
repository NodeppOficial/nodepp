#ifndef NODEPP_ANY
#define NODEPP_ANY

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class any_t {
public: any_t() noexcept {};
    
    any_t( const char* f ) noexcept : any_ptr( new any_impl<string_t>(string::to_string(f)) ) {}
    
    template< class T > 
    any_t( const T& f ) noexcept : any_ptr( new any_impl<T>(f) ) {}

    virtual ~any_t() noexcept = default;
    
    /*─······································································─*/

    ulong count() const noexcept { return any_ptr.count(); }

    bool empty()  const noexcept { return any_ptr.null(); }
    
    /*─······································································─*/

    void operator=( const char* f ) noexcept { set( string::to_string(f) ); }

    template< class T >
    void operator=( const T& f ) noexcept { set( f ); }
    
    /*─······································································─*/

    template< class T >
    T get() const noexcept { T any; any_ptr->get((void*)&any); return any; }

    template< class T >
    void set( const T& f ) noexcept { any_ptr = new any_impl<T>(f); }
    
    /*─······································································─*/

    template< class T >
    explicit operator T(void) const noexcept { return get<T>(); }
    
private:

    class any_base {
    public:
        virtual ~any_base() noexcept {}
        virtual void get( void* /*unused*/ ) const noexcept {}
        virtual void set( void* /*unused*/ )       noexcept {}
    };
    
    /*─······································································─*/
    
    template< class T >
    class any_impl : public any_base {
    public:
        any_impl( const T& f ) noexcept : any( f ) {}
        virtual void get( void* argc ) const noexcept { *((T*)argc) = any; }
        virtual void set( void* argc )       noexcept { any = *((T*)argc); }
    private:
        T any;
    };
    
    /*─······································································─*/
    
    ptr_t<any_base> any_ptr;
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
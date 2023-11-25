#ifndef NODEPP_ANY
#define NODEPP_ANY

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class any_t {
public:
   
    any_t() noexcept = default;
    
    template< class T >
    any_t( T f ) : any_ptr( new any_impl<T>(f) ) {}
    
    any_t( const char* f ) : any_ptr( new any_impl<string_t>(string::to_string(f)) ) {}
    
    /*─······································································─*/

    template< class T >
    T get() const noexcept { T any; any_ptr->get((void*)&any); return any; }

    template< class T >
    void set( T f ){ any_ptr = new any_impl<T>(f); }
    
    /*─······································································─*/

    template< class T >
    explicit operator T(void) const { return get<T>(); }
    
    /*─······································································─*/

    void operator=( const char* f ){ set( string::to_string(f) ); }

    template< class T >
    void operator=( const T& f ){ set( f ); }
    
private:

    class any_base {
    public:
        virtual ~any_base() {}
        virtual void get( void* /*unused*/ ){}
        virtual void set( void* /*unused*/ ){}
    };
    
    /*─······································································─*/
    
    template< class T >
    class any_impl : public any_base {
    public:
        any_impl( T f ) : any( f ) {}
        virtual void get( void* argc ){ *((T*)argc) = any; }
        virtual void set( void* argc ){ any = *((T*)argc); }
    private:
        T any;
    };
    
    /*─······································································─*/
    
    ptr_t<any_base> any_ptr;
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
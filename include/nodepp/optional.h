#ifndef NODEPP_OPTIONAL
#define NODEPP_OPTIONAL

namespace nodepp { 
template< class T > class optional_t {
protected:

    struct NODE {
        bool has; T data;
    };  ptr_t<NODE>obj;

public:

    optional_t( const T& val ) noexcept : obj(new NODE()) {
        obj->has = true; obj->data = val;
    }

    optional_t() noexcept : obj(new NODE()) {
        obj->has = false;
    }

    /*─······································································─*/

    bool has_value() const noexcept { 
        if( obj == nullptr ){ return false; }
            return obj->has;
    }

    /*─······································································─*/

    T& value() { if ( !has_value() ) {
        process::error("Optional does not have a value");
    }   return obj->data; }

    const T& value() const { if ( !has_value() ) {
        process::error("Optional does not have a value");
    }   return obj->data; }
    
};}

#endif
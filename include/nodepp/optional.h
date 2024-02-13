#ifndef NODEPP_OPTIONAL
#define NODEPP_OPTIONAL

namespace nodepp { 
template< class T > class optional_t {
protected:

    bool hasValue; T data;

public:

    optional_t( const T& val ) noexcept : hasValue(true), data(val) {}

    optional_t() noexcept : hasValue(false) {}

    /*─······································································─*/

    bool has_value() const noexcept { return hasValue; }

    /*─······································································─*/

    T& value() {
        if ( !hasValue ) {
            process::error("Optional does not have a value");
        }   return data;
    }

    const T& value() const {
        if ( !hasValue ) {
            process::error("Optional does not have a value");
        }   return data;
    }
    
};}

#endif
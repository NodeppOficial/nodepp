#ifndef NODEPP_EXPECTED
#define NODEPP_EXPECTED

namespace nodepp {
template <typename T, typename E> struct expected_t { 
protected:

    bool hasValue; union {
       T data; E err;
    };

public:

    expected_t( const T& val ) : hasValue(true) , data(val) {}

    expected_t( const E& err ) : hasValue(false), err(err) {}

    /*─······································································─*/

    bool has_value() const { return hasValue; }

    /*─······································································─*/

    T& value() {
        if ( !hasValue ) {
            process::error("expected does not have a value");
        }   return data;
    }

    const T& value() const {
        if ( !hasValue ) {
            process::error("expected does not have a value");
        }   return data;
    }

    /*─······································································─*/

    E& error() {
        if ( hasValue ) {
            process::error("expected does not have an error");
        }   return err;
    }

    const E& error() const {
        if ( hasValue ) {
            process::error("expected does not have an error");
        }   return err;
    }

    /*─······································································─*/

    virtual ~expected_t() {
        if( hasValue ){ data.~T(); } 
        else          { err .~E(); }
    }

};}

#endif
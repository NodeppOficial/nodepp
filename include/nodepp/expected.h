#ifndef NODEPP_EXPECTED
#define NODEPP_EXPECTED

/*────────────────────────────────────────────────────────────────────────────*/

template <typename T, typename E>
struct expected_t { public:

    bool hasValue; union {
       T value;
       E error;
    };

    expected_t( const E& err ) : hasValue(false), error(err) {}

    expected_t( const T& val ) : hasValue(true) , value(val) {}

    virtual ~expected_t() {
        if( hasValue ){ value.~T(); } 
        else          { error.~E(); }
    }

};

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_EXCEPT
#define NODEPP_EXCEPT

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class except_t { 
protected: 

    struct _str_ { 
        void * ev = nullptr;
        string_t msg;
    };  ptr_t<_str_> obj;

public:

    virtual ~except_t() noexcept { 
    //  if ( obj.count() > 2 ){ return; }
   	    process::onSIGERR.off(obj->ev);
    }

    /*─······································································─*/

    template< class T, class = typename type::enable_if<type::is_class<T>::value,T>::type >
    except_t( const T& except_type ) noexcept : obj(new _str_()) {
        obj->msg = except_type.what();
        auto inp = type::bind( this ); 
        obj->ev  = process::onSIGERR.once([=]( ... ){ inp->print(); });
    }

    /*─······································································─*/

    except_t( const string_t& msg ) noexcept : obj(new _str_()) {
        obj->msg = msg;
        auto inp = type::bind( this ); 
        obj->ev  = process::onSIGERR.once([=]( ... ){ inp->print(); });
    }

    /*─······································································─*/

    except_t() noexcept : obj(new _str_()) {
        auto inp = type::bind( this ); 
        obj->msg = "something went wrong";
        obj->ev  = process::onSIGERR.once([=]( ... ){ inp->print(); });
    }

    /*─······································································─*/

    const char* what() const noexcept { return obj->msg.c_str(); }

    operator char*() const noexcept { return (char*)what(); }
    
    void print() const noexcept { console::error(obj->msg); } 

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
#ifndef NODEPP_SERIAL
#define NODEPP_SERIAL

/*────────────────────────────────────────────────────────────────────────────*/

#include "stream.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class serial_t : public stream_t { 
protected:

#ifndef linux
	int _set_baud_rate( string_t path, uint baud ) const noexcept {
		return system( string::format("MODE %s:%u",(char*)path,baud).c_str() );
	}
#else
	int _set_baud_rate( string_t path, uint baud ) const noexcept {
		return system( string::format("stty -F %s %u",(char*)path,baud).c_str() );
	}	
#endif

public:

	serial_t( string_t path, uint baud=9600, const string_t& flag="r+", const ulong& _size=CHUNK_SIZE ){ 
		_set_baud_rate( path, baud ); set_buffer_size( _size ); 
		    v =  fopen( (char*) path, (char*)flag );
		if( v == nullptr ) _Error("such device does not exist");
	}

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace serial {
    template <class... T> serial_t connect( T... args ){ return serial_t( args... ); }
}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
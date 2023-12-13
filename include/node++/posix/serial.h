#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class serial_t : public file_t { 
protected:

	int set_baud_rate( string_t path, uint baud ) const noexcept {
		return ::system( string::format("stty -F %s %u",(char*)path,baud).data());
	}

public: serial_t() noexcept : file_t() {}
    event_t<serial_t> onConnect;

	serial_t( string_t path, uint baud=9600, const string_t& flag="r+", const ulong& _size=CHUNK_SIZE ){ 
		set_baud_rate( path, baud ); obj->fd = open( (char*) path, get_fd_flag(flag) );
		if( obj->fd < 0 ) $Error("such device does not exist");
		set_buffer_size(_size); set_nonbloking_mode();
	}

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace serial {

    template <class... T> serial_t connect( T... args ){ 

		serial_t client( args... ); 

		client.onConnect([=]( serial_t cli ){
			ptr_t<_file_::read> _read = new _file_::read;

			process::task::add([=](){
				if(!cli.is_available() ){ cli.close(); return -1; }
				if((*_read)(&cli)==1 )   { return 1; }
				if(  _read->c <= 0  )    { return 1; }
				cli.onData.emit(_read->y); return 1;
			}); cli.onDrain([=](){ cli.free(); });

		}); 

        process::task::add([=](){
            client.onConnect.emit( client ); return -1;
        }); client.onDrain([=](){ client.free(); });
		
		return client; 
	}

	array_t<string_t> get_devices(){
		array_t<string_t> result; for( auto x : fs::read_folder("/dev") ){
			if( strncmp(x.data(),"ttyUSB",6)==0 || 
				strncmp(x.data(),"ttyACM",6)==0 ||
				strncmp(x.data(),"ttyS"  ,4)==0 
			){  result.push( x );  }
		}	return result;
	}

}}

/*────────────────────────────────────────────────────────────────────────────*/
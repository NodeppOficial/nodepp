/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class serial_t : public file_t { 
protected:

	int set_baud_rate( const string_t& path, uint baud ) const noexcept {
		if( path.empty() || baud == 0 ) { return -1; }
		return ::system( string::format("MODE %s:%u,N,8,1",(char*)path,baud).data() );
	}

public: serial_t() noexcept : file_t() {}
    event_t<serial_t> onConnect;

	serial_t( const string_t& path, uint baud=0, const string_t& mode="r+", const ulong& _size=CHUNK_SIZE ){
		set_baud_rate( path, baud ); auto fg = get_fd_flag( mode ); 
		obj->fd = CreateFileA( path.c_str(), fg[0], fg[1], NULL, fg[2], fg[3], NULL ); 
        if( obj->fd == INVALID_HANDLE_VALUE ) process::error("such file or directory does not exist");
            set_buffer_size( _size ); 
	}

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace serial {

    template <class... T> serial_t connect( const T&... args ){ 

		serial_t client( args... ); 

		client.onConnect.once([=]( serial_t cli ){
			ptr_t<_file_::read> _read = new _file_::read;
			cli.onDrain.once([=](){ cli.free(); });

			process::task::add([=](){
				if(!cli.is_available() )    { cli.close(); return -1; }
				if((*_read)(&cli)==1 )      { return 1; }
				if(  _read->state<=0 )      { return 1; }
				cli.onData.emit(_read->data); return 1;
			});

		}); 

        process::task::add([=](){
            client.onConnect.emit( client ); return -1;
        });
		
		return client; 
	}

	array_t<string_t> get_devices(){
		array_t<string_t> result; for( auto x=0; x<=255; x++ ){
			if( fs::exists_file(string::format("COM%d",x)) )
			  { result.push( string::format("COM%d",x) ); }
		}	return result;
	}

}}

/*────────────────────────────────────────────────────────────────────────────*/
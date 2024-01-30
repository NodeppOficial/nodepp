#include <nodepp/nodepp.h>
#include <nodepp/socket.h>
#include <nodepp/file.h>
#include <nodepp/poll.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

poll_t poll;

/*────────────────────────────────────────────────────────────────────────────*/

void _main_() {

    file_t fileA ( "file.txt", "r" );

    poll.push_read( fileA.get_fd() );

    /*-----------------------------------------*/

    poll.onError([]( int fd ){  });

    poll.onWrite([]( int fd ){  });

    poll.onRead([]( int fd ){  });

    /*-----------------------------------------*/

    process::add([=](){
    _Start
        while( poll.emit() == -1 )
             { _Next; }
    _Stop
    });

}

/*────────────────────────────────────────────────────────────────────────────*/
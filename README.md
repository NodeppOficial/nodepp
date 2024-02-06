## Index

- [Nodepp](#nodepp)
- [Dependencies](#dependencies)
- [Features](#features)
- [Build & Run](#build--run)
    - [Hello World](#hello-world)
    - [Smart Pointers](#smart-pointers)
        - [Basic Pointer](#basic-pointer)
        - [Array Pointer](#array-pointers)
        - [Overload Pointer](#overload-pointers)
    - [Array](#array)
    - [String](#string)
    - [Tasks](#tasks)
    - [Coroutines](#coroutines)
    - [Generators](#generators)
    - [Workers](#workers)
    - [Clusters](#clusters)
    - [Asynchronous Timers](#asynchronous-timers)
        - [Timer Interval](#timer-interval)
        - [Timer Timeout](#timer-timeout)
        - [Timer clear](#timer-clear)
    - [Events](#events)
    - [Observer](#observers)
    - [Promises](#promises)
    - [Streams](#file-stream)
        - [File](#files)
        - [Stream](#streams)
        - [Zlib](#zlib)
    - [TCPSocket](#tcp-socket)
        - [Server](#server)
        - [Client](#client)
    - [TLSSocket](#tls-socket)
        - [server](#server-1)
        - [client](#client-1)
    - [UDPSocket](#udp-socket)
        - [server](#server-2)
        - [client](#client-2)
    - [WebSocket](#websocket)
        - [server](#server-3)
        - [client](#client-3)
    - [HTTP](#http)
        - [server](#server-4)
        - [client](#client-4)
    - [HTTPS](#https)
        - [server](#server-5)
        - [client](#client-5)
    - [Examples](#more-examples-here)
    - [FAQ](#faq)
    - [Contribution](#contribution)
    - [License](#license)

# Nodepp

**Nodepp** is an innovative open source project that aims to make it easier to create applications in **C++**. This project introduces a high-level abstraction layer that allows developers to write **C++** code in a way that is similar to how they would write code in **NodeJS**. With **Nodepp**, developers can leverage the advantages of the **C++** language while benefiting from the ease of use and flexibility that **NodeJS** offers. This unique combination of features makes **Nodepp** a powerful tool for creating high-performance and scalable applications. Additionally, since it is an open source project, anyone can contribute and improve **Nodepp** to fit their specific needs.

- **Nodepp:** compatible with Arduino [here](https://github.com/NodeppOficial/nodepp-arduino)
- **Nodepp:** compatible with PC      [here](https://github.com/NodeppOficial/Nodepp)

## Dependencies
- Openssl `sudo apt install libssl-dev`
- Zlib    `sudo apt install zlib1g-dev`

## Features

- C++11 | C++14 | C++17 | c++20 | c++23 Compatible
- Open source project, meaning anyone can contribute and improve it
- Allows writing **C++** code as if writing in **NodeJS** or **Javascript**
- Includes a **RegExp** engine for processing and manipulating text strings
- Adds a high-level abstraction layer to simplify **C++** application development
- Compatible with several platforms like **Windows | Linux | macOS | freeBSD | Arduino**
- Includes an **Event Loop** that can handle multiple events and tasks on a single thread
- Supports **Coroutines**, which allows running multiple tasks concurrently on a single thread
- Includes support for **TCP | TLS | UDP | HTTP | WS** making it easy to create networked applications
- Includes Support for **Poll | Epoll | Kqueue | WSAPoll** making it easy to handle multiple file descriptors

## Build & Run
- ``gcc -o main main.cpp -lstdc++ -I ./include ; ./main``
- ``g++ -o main main.cpp -I ./include ; ./main``

## Hello world
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

void _main_() {
    console::log("Hello World!");
}
```
```
Hello World!
```

## Smart Pointers
### Basic Pointer
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

void _main_() {
    ptr_t<int> ptr = new int(10);
    console::log( ":>", *ptr );
}
```
```
:> 10
```

### Array Pointers
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

void _main_() {
    ptr_t<int> ptr ({ 10, 20, 30, 40, 50 });
    for( auto x : ptr ) console::log( ":>", x );
}
```
```
:> 10
:> 20
:> 30
:> 40
:> 50
```

### Overload Pointers
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

void _main_() {
    ptr_t<int> ptr = new int(10);
    console::log( ":>", *ptr );

    console::log("----");

    ptr = ptr_t<int>({ 10, 20, 30, 40, 50 });
    for( auto x : ptr ) console::log( ":>", x );
}
```
```
:> 10
----
:> 10
:> 20
:> 30
:> 40
:> 50
```

## Array
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

void _main_() {

    array_t<int> arr ({ 10, 20, 30, 40, 50 });
    for( auto x : arr ) console::log( ":>", x );
}
```
```
:> 10
:> 20
:> 30
:> 40
:> 50
```

## String
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

void _main_() {

    string_t str = "Hello World!";
    console::log( ":>", str );
}
```
```
Hello World!
```

## Tasks
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

void _main_() {

    //return 1 -> will repeat the task
    process::loop::add([=](){
        console::done("repeat mode");
        return  1;
    });

    //return -1 -> will kill the task
    process::loop::add([=](){
        console::error("once mode");
        return -1;
    });

    //return 0 -> will block until != 0
    process::loop::add([=](){
        console::error("block mode");
        return  0;
    });

}
```

## Coroutines
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

void _main_() {

    process::add([=](){ 
        static int x = 3;
    coStart
        while( x --> 0 ){
            console::log("A: x =",x); coNext;
        }
    coStop
    });

    process::add([=](){ 
        static int x = 3;
    coStart
        while( x --> 0 ){
            console::log("B: x =",x); coNext;
        }
    coStop
    });

}
```
```
A: x = 2
B: x = 2
A: x = 1
B: x = 1
A: x = 0
B: x = 0
```

## Generators
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

GENERATOR( gen ) {
    
    int x = 3;

    gnEmit(){ 
    gnStart;
        while( x-->0 ){
            console::log("x:>",x); 
            coNext;
        }
    gnStop;
    }

};

void _main_() { 
    gen A; process::add(A); 
}
```
```
x:> 2
x:> 1
x:> 0
```

## Workers
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/worker.h>
#include <nodepp/timer.h>

using namespace nodepp;

void _main_() {

    worker::add([](){
        static int x = 3;
    coStart
        while( x --> 0 ){
            console::log("x: x =",x); coNext;
        }
    coStop
    });

}
```
```
x:> 2
x:> 1
x:> 0
```

## Clusters
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/cluster.h>

using namespace nodepp;

void _main_() {

    auto x = cluster::add();

    if( cluster::is_child() ){
        console::log("child");
    } else {
        console::log("parent");
    }

}
```
```
parent
child
```

## Asynchronous Timers
### Timer Interval
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/timer.h>

using namespace nodepp;

void _main_() {

    timer::interval([=](){ 
        static int i=0; i++; i %= 1000;
        console::done("interval every: 1 second - ",i," seconds");
    },1000);

    console::log("Hello World!");

}
```
```
Hello World!
interval every: 1 second - 1 seconds
interval every: 1 second - 2 seconds
interval every: 1 second - 3 seconds
interval every: 1 second - 4 seconds
...
```

### Timer Timeout
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/timer.h>

using namespace nodepp;

void _main_() {

    timer::timeout([=](){
        console::error("timeout after: 10 seconds");
    },10000);

    console::log("Hello World!");

}
```
```
Hello World!
timeout after: 10 seconds
```

### Timer Clear
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/timer.h>

using namespace nodepp;

void _main_() {

    auto x = timer::timeout([=](){
        console::error("timeout after: 10 seconds");
    },10000);

    console::log("Hello World!");

    timer::clear( x );

}
```
```
Hello World!
```

## Events
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/event.h>

using namespace nodepp;

event_t<> ev; 

void _main_() {

  ev.on([](){ console::log("Hello World!"); });
  ev.emit();

}
```
```
Hello World!
```

## Observers
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/observer.h>

using namespace nodepp;

observer_t obj ({
    { "a", 0 },
    { "b", 0 }
});

void _main_() {

    obj.on( "b", []( any_t A, any_t B ){
        console::done( " B:>", (int)A, "|", (int)B, "changed" );
    });

    obj.on( "a", []( any_t A, any_t B ){
        console::error( "A:>", (int)A, "|", (int)B, "changed" );
    });
    
    obj.set( "a", 20 );
    obj.set( "b", 10 );

}
```
```
A:> 0 | 20 changed
B:> 0 | 10 changed
```

## Promises
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <nodepp/promise.h>

using namespace nodepp;

void _main_() {

    promise_t<int,except_t>([=]( function_t<void,int> res,  function_t<void,except_t> rej ){
        timer::timeout([=](){ res(10); },1000);
    })
    
    .then([=]( int res ){
        console::log( ":>", res );
    })
    
    .fail([=]( except_t rej ){
        console::log( ":>", rej );
    });

    console::log("Hello World!");

}
```
```
Hello World!
:> 10
```

## File Stream
### files
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/stream.h>

using namespace nodepp;

void _main_() {

    //file as write mode
    file_t fw ( "PATH", "w" );
    fw.write("Hello World!");
    
    //file as read mode
    file_t fr ( "PATH", "r" );
    fr.read();
    
}
```

### streams
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/stream.h>

using namespace nodepp;

void _main_() {

    file_t fw ( "PATHA", "w" );
    file_t fr ( "PATHB", "r" );

    stream::pipe( fr, fw );    
    
}
```

### zlib
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/stream.h>
#include <nodepp/zlib.h>

using namespace nodepp;

void _main_() {

    file_t fw ( "PATHA", "w" );
    file_t fr ( "PATHB", "r" );

    stream::gzpi::pipe( fr, fw );    
    
}
```

## TCP Socket
### Server
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/tcp.h>

using namespace nodepp;

void _main_() {

    auto server = tcp::server();

    server.onConnect([=]( socket_t cli ){
    
        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose([=](){
            console::log("closed");
        });

    });

    server.listen( "localhost", 8000, []( socket_t srv ){
        console::log("server started at tcp://localhost:8000");
    });

}
```

### Client 
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/tcp.h>

using namespace nodepp;

void _main_() {

    auto client = tcp::client();

    client.onOpen([=]( socket_t cli ){
    
        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose.once([=](){
            console::log("closed");
        });

        cli.write("Hello World");

    });

    client.connect( "localhost", 8000, []( socket_t cli ){
        console::log("client COnnected to tcp://localhost:8000");
    });

}
```

## TLS Socket
### Server
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/tls.h>

using namespace nodepp;

ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

void _main_() {

    auto server = tls::server( &ssl );

    server.onConnect([=]( ssocket_t cli ){

        console::log("connected to:", cli.get_fd());
    
        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose([=](){
            console::log("closed");
        });

    });

    server.listen( "localhost", 8000, []( ssocket_t srv ){
        console::log("server started at tls://localhost:8000");
    });

}
```

### Client
```cpp

#include <nodepp/nodepp.h>
#include <nodepp/tls.h>

using namespace nodepp;

ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

void _main_() {

    auto client = tls::client( &ssl );

    client.onOpen([=]( socket_t cli ){
    
        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose.once([=](){
            console::log("closed");
        });

        cli.write("Hello World!");

    });

    client.connect( "localhost", 8000, []( socket_t cli ){
        console::log("client Connected to tls://localhost:8000");
    });

}
```

## UDP Socket
### Server
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/udp.h>

using namespace nodepp;

void _main_() {

    auto server = udp::server();

    server.onConnect([=]( socket_t cli ){
    
        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose([=](){
            console::log("closed");
        });

    });

    server.listen( "localhost", 8000, []( socket_t srv ){
        console::log("server started at udp://localhost:8000");
    });

}
```

### Client
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/udp.h>

using namespace nodepp;

void _main_() {

    auto client = udp::client();

    client.onOpen([=]( socket_t cli ){
    
        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose.once([=](){
            console::log("closed");
        });

        cli.write("Hello World!");

    });

    client.connect( "localhost", 8000, []( socket_t cli ){
        console::log("client connected to udp://localhost:8000");
    });

}
```

## WebSocket
### Server
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/ws.h>

using namespace nodepp;

void _main_() {

    auto server = ws::server();

    server.onConnect([]( ws_t cli ){

        console::log("connected");

        cli.onData([=]( string_t data ){
            console::log( data );
        });

        cli.onClose([=](){
            console::log("closed");
        });

    });

    server.listen( "localhost", 8000, []( ... ){
        console::log("server started at ws://localhost:8000");
    });

}
```

### Client
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/ws.h>

using namespace nodepp;

void _main_() {

    auto client = ws::client( "ws://localhost:8000/" );
    
    client.onOpen([=](){ 
        
        console::log("connected"); 

        client.write("Hello World!");

        client.onClose([](){ console::log("closed"); });

    });

    client.onError([=]( except_t err ){
        console::log(err);
    });

}
```

## HTTP
### Server
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/http.h>
#include <nodepp/date.h>

using namespace nodepp;

void _main_() {

    auto server = http::server([=]( http_t cli ){ 

        console::log( cli.path, cli.get_fd() );
        
        cli.write_headers( 200, {{
            { "content-type", "text/html" }
        }});
        
        cli.write( date::fulltime() );
        cli.close();

    });

    server.listen( "localhost", 8000, [=]( socket_t server ){
        console::log("server started at http://localhost:8000");
    });

}
```

### client
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/http.h>

using namespace nodepp;

void _main_() {

    fetch_t args;
            args.method = "GET";
            args.url = "https://www.google.com/";
            args.headers = {{
                { "Host", url::host(args.url) }
            }};
        //  args.file = file_t("PATH","r");
        //  args.body = "MYBODY";

    http::fetch( args )

    .then([]( http_t cli ){
        cli.onData([]( string_t chunk ){
            console::log( chunk );
        }); stream::pipe( cli );
    })

    .fail([]( except_t err ){
        console::error( err );
    });

}
```

## HTTPS
### Server
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/https.h>
#include <nodepp/date.h>

using namespace nodepp;

void _main_() {

    ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

    auto server = https::server([=]( https_t cli ){

        console::log( cli.path, cli.get_fd() );
        
        cli.write_headers( 200, {{
            { "content-type", "text/html" }
        }});
        
        cli.write( date::fulltime() );
        cli.close();

    }, &ssl );

    server.listen( "localhost", 8000, [=]( ssocket_t server ){
        console::log("server started at https://localhost:8000");
    });

}
```

### Client
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/https.h>

using namespace nodepp;

void _main_() {

    ssl_t ssl( "./ssl/key.pem", "./ssl/cert.pem" );

    fetch_t args;
            args.method = "GET";
            args.url = "https://www.google.com/";
            args.headers = {{
                { "Host", url::host(args.url) }
            }};
        //  args.file = file_t("PATH","r");
        //  args.body = "MYBODY";

    https::fetch( args, &ssl )

    .then([]( https_t cli ){
        cli.onData([]( string_t chunk ){
            console::log( chunk.size(), ":>", chunk );
        }); stream::pipe( cli );
    })

    .fail([]( except_t err ){
        console::error( err );
    });

}
```

## More Examples Here
- Examples    : [here](https://github.com/NodeppOficial/Nodepp/tree/main/examples)

## FAQ
- reddit : [/r/Cplusplus/](https://www.reddit.com/r/Cplusplus/comments/19e2kw3/write_asynchronous_code_with_c_nodepp/) 
- reddit : [/r/cpp/](https://www.reddit.com/r/cpp/comments/19camli/write_asynchronously_code_with_c_nodepp/)
  
## Contribution

If you want to contribute to **Nodepp**, you are welcome to do so! You can contribute in several ways:

- Improving the documentation
- Reporting bugs and issues
- Adding new features or improving existing ones
- Writing tests and ensuring compatibility with different platforms
- Before submitting a pull request, make sure to read the contribution guidelines.

## License

**Nodepp** is distributed under the MIT License. See the LICENSE file for more details.
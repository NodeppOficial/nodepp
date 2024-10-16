# Nodepp

**Nodepp** is an innovative open source project that aims to make it easier to create applications in **C++**. This project introduces a high-level abstraction layer that allows developers to write **C++** code in a way that is similar to how they would write code in **NodeJS**. With **Nodepp**, developers can leverage the advantages of the **C++** language while benefiting from the ease of use and flexibility that **NodeJS** offers. This unique combination of features makes **Nodepp** a powerful tool for creating high-performance and scalable applications. Additionally, since it is an open source project, anyone can contribute and improve **Nodepp** to fit their specific needs.

## Dependencies
```bash
# Openssl
    🪟: pacman -S mingw-w64-ucrt-x86_64-openssl
    🐧: sudo apt install libssl-dev

# Zlib
    🪟: pacman -S mingw-w64-ucrt-x86_64-zlib
    🐧: sudo apt install zlib1g-dev
```

## Features

- 📌: **C++11 | C++14 | C++17 | c++20 | c++23** Compatible
- 📌: Allows writing **C++** code as if writing in **NodeJS** or **Javascript**
- 📌: Adds a high-level abstraction layer to simplify **C++** application development
- 📌: Compatible with several platforms like **Windows | Linux | macOS | freeBSD**
- 📌: Supports **Coroutines**, which allows running multiple tasks concurrently on a single thread

## Bateries Included

- 📌: Include a **build-in JSON** parser / stringify system.
- 📌: Include a **build-in RegExp** engine for processing text strings.
- 📌: Include a **build-in System** that make every object **Async Task** safety.
- 📌: Include a **Smart Pointer** base **Garbage Collector** to avoid **Memory Leaks**.
- 📌: Include support for **Reactive Programming** based on **Events** and **Observers**.
- 📌: Include an **Event Loop** that can handle multiple events and tasks on a single thread.
- 📌: Include support for **TCP | TLS | UDP | HTTP | WS** making it easy to create networked applications.
- 📌: Include Support for **Poll | Epoll | Kqueue | WSAPoll** making it easy to handle multiple file descriptors.

## Build & Run
```bash
🐧: g++ -o main main.cpp -I ./include ; ./main
🪟: g++ -o main main.cpp -I ./include -lws2_32 ; ./main
```

## Examples
### Hello world
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

void onMain() {
    console::log("Hello World!");
}
```

### HTTP Client
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/http.h>

using namespace nodepp;

void onMain(){

    fetch_t args;
            args.method = "GET";
            args.url = "http://www.google.com/";
            args.headers = header_t({
                { "Host", url::host(args.url) }
            });

    http::fetch( args )

    .then([]( http_t cli ){
        console::log( stream::await( cli ) );
    })

    .fail([]( except_t err ){
        console::error( err );
    });

}
```

### HTTP Server
```cpp
#include <nodepp/nodepp.h>
#include <nodepp/http.h>
#include <nodepp/date.h>

using namespace nodepp;

void onMain(){

    auto server = http::server([=]( http_t cli ){ 

        console::log( cli.path, cli.get_fd() );
        
        cli.write_header( 200, header_t({
            { "content-type", "text/html" }
        }));
        
        cli.write( date::fulltime() );
        cli.close(); // optional | GC automaticaly close unused sockets

    });

    server.listen( "localhost", 8000, [=]( socket_t server ){
        console::log("server started at http://localhost:8000");
    });

}
```

### More Examples [here](https://github.com/NodeppOficial/Nodepp/tree/main/examples)

## Projects made with NodePP
- 🔗: [Draw on your PC using your smartphone](https://github.com/ScreenDraw/PCDraw)
- 🔗: [Simple multiplayer Game With Raylib](https://medium.com/@EDBCBlog/create-your-own-online-multiplayer-small-fast-and-fun-with-raylib-nodepp-and-websockets-190f5c174094)
- 🔗: [Cursed Luna - A simple Raylib Game](https://github.com/EDBCREPO/Space-Shocker)
- 🔗: [Serial Port arduino using Nodepp](https://github.com/EDBCREPO/Arduino_PC)
- 🔗: [Simple Raylib Real-Time Chat](https://github.com/EDBCREPO/simple-raylib-websocket-chat)

Check out some articles on [Medium](https://medium.com/@EDBCBlog)

## Compatibility
- 🔗: [NodePP for Window | Linux | Mac | Bsd ](https://github.com/NodeppOficial/nodepp)
- 🔗: [NodePP for Arduino](https://github.com/NodeppOficial/nodepp-arduino)
- 🔗: [Nodepp for WASM](https://github.com/NodeppOficial/nodepp-wasm)
 
## FAQ
- 🔗 : [/r/Nodepp/](https://www.reddit.com/r/Nodepp/comments/1eaq1pu/faq_ask_anything_about_nodepp/)
  
## Contribution

If you want to contribute to **Nodepp**, you are welcome to do so! You can contribute in several ways:

- ☕ Buying me a Coffee
- 📢 Reporting bugs and issues
- 📝 Improving the documentation
- 📌 Adding new features or improving existing ones
- 🧪 Writing tests and ensuring compatibility with different platforms
- 🔍 Before submitting a pull request, make sure to read the contribution guidelines.

**NOTE:** We don't use STD here, just Pure C++.

## License

**Nodepp** is distributed under the MIT License. See the LICENSE file for more details.

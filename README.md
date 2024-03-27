# Nodepp

<img src="https://github.com/NodeppOficial/nodepp/assets/109326461/241da5f3-ae97-4d02-8fa0-87e90c6afcbf" 
     width="180px" align="left" style=" padding:10px; " >

**Nodepp** is an innovative open source project that aims to make it easier to create applications in **C++**. This project introduces a high-level abstraction layer that allows developers to write **C++** code in a way that is similar to how they would write code in **NodeJS**. With **Nodepp**, developers can leverage the advantages of the **C++** language while benefiting from the ease of use and flexibility that **NodeJS** offers. This unique combination of features makes **Nodepp** a powerful tool for creating high-performance and scalable applications. Additionally, since it is an open source project, anyone can contribute and improve **Nodepp** to fit their specific needs.

## Dependencies
- Openssl `sudo apt install libssl-dev`
- Zlib    `sudo apt install zlib1g-dev`

## Features

- **C++11 | C++14 | C++17 | c++20 | c++23** Compatible
- Allows writing **C++** code as if writing in **NodeJS** or **Javascript**
- Open source project, meaning anyone can contribute and improve it
- Includes a **RegExp** engine for processing and manipulating text strings
- Adds a high-level abstraction layer to simplify **C++** application development
- Compatible with several platforms like **Windows | Linux | macOS | freeBSD | Arduino**
- Includes an **Event Loop** that can handle multiple events and tasks on a single thread
- Supports **Coroutines**, which allows running multiple tasks concurrently on a single thread
- Includes support for **TCP | TLS | UDP | HTTP | WS** making it easy to create networked applications
- Includes Support for **Poll | Epoll | Kqueue | WSAPoll** making it easy to handle multiple file descriptors

## Build & Run
- Posix:   `g++ -o main main.cpp -I ./include ; ./main`
- Windows: `g++ -o main main.cpp -I -lws2_32 ./include ; ./main`

## Hello world
```cpp
#include <nodepp/nodepp.h>

using namespace nodepp;

onMain([](){
    console::log("Hello World!");
})
```
```
Hello World!
```

## Examples
- Examples : [here](https://github.com/NodeppOficial/Nodepp/tree/main/examples)

## FAQ
- reddit : [/r/Cplusplus/](https://www.reddit.com/r/Cplusplus/comments/19e2kw3/write_asynchronous_code_with_c_nodepp/)
  
## Contribution

If you want to contribute to **Nodepp**, you are welcome to do so! You can contribute in several ways:

- Buying me a Coffee
- Reporting bugs and issues
- Improving the documentation
- Adding new features or improving existing ones
- Writing tests and ensuring compatibility with different platforms
- Before submitting a pull request, make sure to read the contribution guidelines.

NOTE: We don't use STD here, just Pure C++.

## License

**Nodepp** is distributed under the MIT License. See the LICENSE file for more details.

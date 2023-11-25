# Node++

**Node++** is an innovative open source project that aims to make it easier to create applications in **C++**. This project introduces a high-level abstraction layer that allows developers to write **C++** code in a way that is similar to how they would write code in **NodeJS**. With **Node++**, developers can leverage the advantages of the **C++** language while benefiting from the ease of use and flexibility that **NodeJS** offers. This unique combination of features makes **Node++** a powerful tool for creating high-performance and scalable applications. Additionally, since it is an open source project, anyone can contribute and improve **Node++** to fit their specific needs.

- **uNode++:** compatible with embed devices [here](https://github.com/EDBCREPO/uNODEPP)
- **Node++:** compatible with PC and esp32  [here](https://github.com/EDBCREPO/NODEPP)

## Dependencies
- Openssl `sudo apt install libssl-dev`
- Zlib    `sudo apt install zlib1g-dev`

## Key Features

- Allows writing C++ code as if writing in NodeJS
- Adds a high-level abstraction layer to simplify C++ application development
- Includes an event loop that can handle multiple events and tasks on a single thread of execution
- Supports coroutines, which allows running multiple tasks concurrently on a single thread
- Includes support for TCP, TLS, UDP, HTTP, and HTTPS, making it easy to create networked applications
- Includes a regular expression engine for processing and manipulating text strings
- Compatible with several platforms, including Windows, macOS, Linux, ESP32, and ESP8266
- Open source project, meaning anyone can contribute and improve it
- Has extensive documentation and an active community of developers willing to help

## Installation

```bash
  g++ -o main main.cpp -I ./include ; ./main
```

## Usage

To use **Node++**, simply include the `node++.h` header file in your project and start writing **C++** code as if you were writing in **NodeJS**. Here's a simple example:

```cpp
  #include <node++/node++.h>

  using namespace nodepp;

  void _Ready() {
    console::log("Hello World!");
  }
```

Examples - [here](https://github.com/EDBCREPO/NODEPP/tree/main/examples)
  
## Contribution

If you want to contribute to **Node++**, you are welcome to do so! You can contribute in several ways:

- Improving the documentation
- Reporting bugs and issues
- Adding new features or improving existing ones
- Writing tests and ensuring compatibility with different platforms
- Before submitting a pull request, make sure to read the contribution guidelines.

## License

**Node++** is distributed under the MIT License. See the LICENSE file for more details.

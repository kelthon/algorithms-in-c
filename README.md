# algorithms-in-c

This repository provides implementations of fundamental data structure algorithms in C — including linked lists, stacks, and queues — organized as a C static library using CMake. It was developed during the Computer Science course at Universidade Federal do Cariri (UFCA) to consolidate learning, maintain well‑structured code, and serve as a reliable reference for future studies and projects.

## Installation Guide

First, ensure you have CMake and any C compile (e.g GCC or Clang) installed on your system. Then, follow the steps below to build and install the library:

### 1. Downloading source code

Clone this repository from github:

```bash
git clone https://github.com/kelthon/algorithms-in-c.git
```

### 2. Changing to the build directory

Make sure you are in the root directory of the cloned repository, then create a build directory and navigate into it:

```bash
cd algorithms-in-c
```

### 3. Building the Library

Use CMake to configure the build system and then compile the library:

```bash
cmake -S . -B build
cmake --build build
```

## Usage

Include the library in your C project and link against it to use the provided data structures.

### 1. Example Usage

```c
#include <aic.h>

int main(int argc, char *argv[]) {
  List *numbers = list_create();

  for (size_t i = 0; i < 10; i++) {
    list_append(numbers, i);
  }
  
  list_delete(numbers, NULL);
  return 0;
}
```

### Linking the Library

When compiling your C project, link against the built library. For example, if you are using `gcc`, you can do:

```bash
gcc -o my_program my_program.c -L /path/to/algorithms-in-c/build -laic
```

## Contributing

1. Fork and Clone
1. Create a new branch
1. Add your changes
1. Commit and push
1. Open a Pull Request

## License

This repository is licensed using [MIT license](./LICENSE)

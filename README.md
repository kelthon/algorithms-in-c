# algorithms-in-c

This repository contains implementations of data structure algorithms in C, such linked lists, stacks and queues, packaged into a C static lib using CMake. It was created to store and organize code written during the Computer Science course at Universidade Federal do Cariri (UFCA), serving as a reference for future studies and projects.

## ⚙️ Installation

To install, clone this repository and install using cmake

```bash
git clone https://github.com/kelthon/algorithms-in-c.git
cd algorithms-in-c
cmake install .
```

## 💡 Usage

Include the library in your C project and link against it to use the provided data structures.

```C
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

## 🤝Contributing

1. Fork and Clone
1. Create a new branch
1. Add your changes
1. Commit and push
1. Open a Pull Request

## 📄 License

This repository is licensed using [MIT license](./LICENSE)

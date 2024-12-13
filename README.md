# Concept #

A C++ library providing random features.

# Install #
## Requirements ##

Binaries:

- A C++20 compiler (ex: g++-13)
- CMake 3.26 or later

Testing Libraries (optional):

- [Google Test](https://github.com/google/googletest) 1.13 or later (optional)

## Clone

```
git clone https://github.com/arapelle/arba-rand
```

## Quick Install ##
There is a cmake script at the root of the project which builds the library in *Release* mode and install it (default options are used).
```
cd /path/to/arba-rand
cmake -P cmake/scripts/quick_install.cmake
```
Use the following to quickly install a different mode.
```
cmake -P cmake/scripts/quick_install.cmake -- TESTS BUILD Debug DIR /tmp/local
```

## Uninstall ##
There is a uninstall cmake script created during installation. You can use it to uninstall properly this library.
```
cd /path/to/installed-arba-rand/
cmake -P uninstall.cmake
```

# How to use
## Example - Compute a neutral murmur rand with 64 bits
```c++
#include <arba/rand/rand.hpp>
#include <arba/rand/version.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << rand::rand_i64() << std::endl;
    std::cout << "EXIT SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
```

# License

[MIT License](./LICENSE.md) © arba-rand

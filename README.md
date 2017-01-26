# GTest -light [![Build Status](https://travis-ci.org/dubzzz/gtest-light.svg?branch=master)](https://travis-ci.org/dubzzz/gtest-light)

_Unit-test your devs on Online compilers_

Light version of GTest framework compatible with Online compilers like ideone, godbolt, codingame

## Disclaimer

Please note the following before using this project:
- not supposed to replace GoogleTest
- only a very limited subset implemented
- code is not supposed to be really readable: short file in order to use easily in online compilers

## Requirements 

Compiler with C++11 enabled.

## How to use it?

Simply copy-and-paste the content of gtest_light.hpp into your source file.
Resulting file might look like:

```cpp
/* Your code goes here */

/* Content of gtest_light.hpp */

/* Your GTest unit-tests */

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int ret { RUN_ALL_TESTS() };
  return ret;
}
```

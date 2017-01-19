# GTest -light

_Unit-test your devs on Online compilers_

Light version of GTest framework compatible with Online compilers like ideone, godbolt, codingame

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

message(STATUS "Scanning test files")
file(GLOB SRCS *.cpp)
foreach(TEST_SRC ${SRCS})
  get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_SRC} ../gtest_light.hpp ../analysis/check.hpp)
  add_test(${TEST_NAME} COMMAND ${TEST_NAME})
  message(STATUS "Added test: ${TEST_NAME}")
endforeach()


#include <sstream>
static std::ostringstream& get_stream()
{
  static std::ostringstream oss;
  return oss;
}
#define TEST_LOGGER() get_stream()
#include "../gtest_light.hpp"
#include "../analysis/check.hpp"

#include <string>
#include <vector>

TEST(CallOnce, OnSuccess)
{
  int val {};
  auto fun = [val](int incr) mutable { val += incr; return val; };
  
  EXPECT_EQ(fun(10), 10);
  EXPECT_EQ(fun(10), 20);
}

TEST(CallOnce, OnFailure)
{
  int val {};
  auto fun = [val](int incr) mutable { val += incr; return val; };
  
  EXPECT_EQ(fun(10), 0);
  EXPECT_EQ(fun(10), 0);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int ret { RUN_ALL_TESTS() };
  
  std::vector<::analysis::Test> expected;
  expected.push_back(::analysis::Test("CallOnce", "OnSuccess"));
  expected.push_back(::analysis::Test("CallOnce", "OnFailure")
      .withFailure(28, ::analysis::Level::Expect, ::analysis::Operation::Eq, "10", "0")
      .withFailure(29, ::analysis::Level::Expect, ::analysis::Operation::Eq, "20", "0"));
  
  return ::analysis::check(ret, get_stream().str(), expected);
}


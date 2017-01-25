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

TEST(AssertEq, OnlySuccess)
{
  ASSERT_EQ(1, 1);
  ASSERT_EQ(std::string("toto"), std::string("toto"));
}

TEST(AssertEq, ImmediateFailure)
{
  ASSERT_EQ(2, 1);
}

TEST(AssertEq, AssertStopExecution)
{
  ASSERT_EQ(2, 1);
  ASSERT_EQ(1, 1);
  ASSERT_EQ(3, 1);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int ret { RUN_ALL_TESTS() };
  
  std::vector<::analysis::Test> expected;
  expected.push_back(::analysis::Test("AssertEq", "OnlySuccess"));
  expected.push_back(::analysis::Test("AssertEq", "ImmediateFailure")
      .withFailure(22, ::analysis::Level::Assert, ::analysis::Operation::Eq, "2", "1"));
  expected.push_back(::analysis::Test("AssertEq", "AssertStopExecution")
      .withFailure(27, ::analysis::Level::Assert, ::analysis::Operation::Eq, "2", "1"));
  
  return ::analysis::check(ret, get_stream().str(), expected);
}


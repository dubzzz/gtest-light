#include <sstream>
static std::ostringstream& get_stream()
{
  static std::ostringstream oss;
  return oss;
}
#define TEST_LOGGER() get_stream()
#include "../gtest_light.hpp"
#include "../analysis/check.hpp"

#include <memory>
#include <string>
#include <vector>

TEST(AssertFailure, ASSERT_EQ)
{
  ASSERT_EQ(21, 42);
  ASSERT_EQ(std::string("bob"), std::string("bob"));
  ASSERT_EQ(std::vector<int>(5, 2), (std::vector<int>{2,2,2,2,2}));
  ASSERT_EQ([](std::size_t v) { return v; }(42), std::string("01234567890123456789").size());
}

TEST(AssertFailure, ASSERT_NE)
{
  ASSERT_NE(21, 42);
  ASSERT_NE(std::vector<int>(5, 2), (std::vector<int>{2,2,2,2,2}));
  ASSERT_NE(std::string("foo"), std::string("foo"));
}

TEST(AssertFailure, ASSERT_TRUE)
{
  ASSERT_TRUE(true);
  ASSERT_TRUE(5+8);
  ASSERT_TRUE(std::unique_ptr<int>());
  ASSERT_TRUE(std::string().empty());
  ASSERT_TRUE(std::string("bob").empty());
}

TEST(AssertFailure, ASSERT_FALSE)
{
  ASSERT_FALSE(true);
  ASSERT_FALSE(42 == 42);
  ASSERT_FALSE(! std::string().empty());
  ASSERT_FALSE(std::string("bob").empty());
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int ret { RUN_ALL_TESTS() };
  
  std::vector<::analysis::Test> expected;
  expected.push_back(::analysis::Test("AssertFailure", "ASSERT_EQ")
      .withFailure(17, ::analysis::Level::Assert, ::analysis::Operation::Eq, "21", "42"));
  expected.push_back(::analysis::Test("AssertFailure", "ASSERT_NE")
      .withFailure(26, ::analysis::Level::Assert, ::analysis::Operation::Ne, ::analysis::to_string(std::vector<int>(5, 2)), ::analysis::to_string(std::vector<int>{2,2,2,2,2})));
  expected.push_back(::analysis::Test("AssertFailure", "ASSERT_TRUE")
      .withFailure(34, ::analysis::Level::Assert, ::analysis::Operation::True, "true", ::analysis::to_string(std::unique_ptr<int>())));
  expected.push_back(::analysis::Test("AssertFailure", "ASSERT_FALSE")
      .withFailure(41, ::analysis::Level::Assert, ::analysis::Operation::False, "false", "true"));
  
  return ::analysis::check(ret, get_stream().str(), expected);
}


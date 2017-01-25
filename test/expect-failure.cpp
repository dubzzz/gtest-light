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

TEST(ExpectFailure, EXPECT_EQ)
{
  EXPECT_EQ(21, 42);
  EXPECT_EQ(std::string("bob"), std::string("bob"));
  EXPECT_EQ(std::vector<int>(5, 2), (std::vector<int>{2,2,2,2,2}));
  EXPECT_EQ([](std::size_t v) { return v; }(42), std::string("01234567890123456789").size());
}

TEST(ExpectFailure, EXPECT_NE)
{
  EXPECT_NE(21, 42);
  EXPECT_NE(std::vector<int>(5, 2), (std::vector<int>{2,2,2,2,2}));
  EXPECT_NE(std::string("foo"), std::string("foo"));
}

TEST(ExpectFailure, EXPECT_TRUE)
{
  EXPECT_TRUE(true);
  EXPECT_TRUE(5+8);
  EXPECT_TRUE(std::unique_ptr<int>());
  EXPECT_TRUE(std::string().empty());
  EXPECT_TRUE(std::string("bob").empty());
}

TEST(ExpectFailure, EXPECT_FALSE)
{
  EXPECT_FALSE(true);
  EXPECT_FALSE(42 == 42);
  EXPECT_FALSE(! std::string().empty());
  EXPECT_FALSE(std::string("bob").empty());
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int ret { RUN_ALL_TESTS() };
  
  std::vector<::analysis::Test> expected;
  expected.push_back(::analysis::Test("ExpectFailure", "EXPECT_EQ")
      .withFailure(17, ::analysis::Level::Expect, ::analysis::Operation::Eq, "21", "42")
      .withFailure(20, ::analysis::Level::Expect, ::analysis::Operation::Eq, "42", "20"));
  expected.push_back(::analysis::Test("ExpectFailure", "EXPECT_NE")
      .withFailure(26, ::analysis::Level::Expect, ::analysis::Operation::Ne, ::analysis::to_string(std::vector<int>(5, 2)), ::analysis::to_string(std::vector<int>{2,2,2,2,2}))
      .withFailure(27, ::analysis::Level::Expect, ::analysis::Operation::Ne, "foo", "foo"));
  expected.push_back(::analysis::Test("ExpectFailure", "EXPECT_TRUE")
      .withFailure(34, ::analysis::Level::Expect, ::analysis::Operation::True, "true", ::analysis::to_string(std::unique_ptr<int>()))
      .withFailure(36, ::analysis::Level::Expect, ::analysis::Operation::True, "true", "false"));
  expected.push_back(::analysis::Test("ExpectFailure", "EXPECT_FALSE")
      .withFailure(41, ::analysis::Level::Expect, ::analysis::Operation::False, "false", "true")
      .withFailure(42, ::analysis::Level::Expect, ::analysis::Operation::False, "false", "true"));
  
  return ::analysis::check(ret, get_stream().str(), expected);
}


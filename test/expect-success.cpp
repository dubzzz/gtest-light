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

TEST(ExpectSuccess, EXPECT_EQ)
{
  EXPECT_EQ(42, 42);
  EXPECT_EQ(std::string("bob"), std::string("bob"));
  EXPECT_EQ(std::vector<int>(5, 2), (std::vector<int>{2,2,2,2,2}));
  EXPECT_EQ([](std::size_t v) { return v-22; }(42), std::string("01234567890123456789").size());
}

TEST(ExpectSuccess, EXPECT_NE)
{
  EXPECT_NE(21, 42);
  EXPECT_NE(std::string("foo"), std::string("bar"));
}

TEST(ExpectSuccess, EXPECT_TRUE)
{
  EXPECT_TRUE(true);
  EXPECT_TRUE(42);
  EXPECT_TRUE(std::string().empty());
  EXPECT_TRUE(! std::string("bob").empty());
}

TEST(ExpectSuccess, EXPECT_FALSE)
{
  EXPECT_FALSE(false);
  EXPECT_FALSE(42 == 21);
  EXPECT_FALSE(! std::string().empty());
  EXPECT_FALSE(std::string("bob").empty());
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int ret { RUN_ALL_TESTS() };
  
  std::vector<::analysis::Test> expected;
  expected.push_back(::analysis::Test("ExpectSuccess", "EXPECT_EQ"));
  expected.push_back(::analysis::Test("ExpectSuccess", "EXPECT_NE"));
  expected.push_back(::analysis::Test("ExpectSuccess", "EXPECT_TRUE"));
  expected.push_back(::analysis::Test("ExpectSuccess", "EXPECT_FALSE"));
  
  return ::analysis::check(ret, get_stream().str(), expected);
}


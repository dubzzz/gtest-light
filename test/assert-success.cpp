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

TEST(AssertSuccess, ASSERT_EQ)
{
  ASSERT_EQ(42, 42);
  ASSERT_EQ(std::string("bob"), std::string("bob"));
  ASSERT_EQ(std::vector<int>(5, 2), (std::vector<int>{2,2,2,2,2}));
  ASSERT_EQ([](std::size_t v) { return v-22; }(42), std::string("01234567890123456789").size());
}

TEST(AssertSuccess, ASSERT_NE)
{
  ASSERT_NE(21, 42);
  ASSERT_NE(std::string("foo"), std::string("bar"));
}

TEST(AssertSuccess, ASSERT_TRUE)
{
  ASSERT_TRUE(true);
  ASSERT_TRUE(42);
  ASSERT_TRUE(std::string().empty());
  ASSERT_TRUE(! std::string("bob").empty());
}

TEST(AssertSuccess, ASSERT_FALSE)
{
  ASSERT_FALSE(false);
  ASSERT_FALSE(42 == 21);
  ASSERT_FALSE(! std::string().empty());
  ASSERT_FALSE(std::string("bob").empty());
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int ret { RUN_ALL_TESTS() };
  
  std::vector<::analysis::Test> expected;
  expected.push_back(::analysis::Test("AssertSuccess", "ASSERT_EQ"));
  expected.push_back(::analysis::Test("AssertSuccess", "ASSERT_NE"));
  expected.push_back(::analysis::Test("AssertSuccess", "ASSERT_TRUE"));
  expected.push_back(::analysis::Test("AssertSuccess", "ASSERT_FALSE"));
  
  return ::analysis::check(ret, get_stream().str(), expected);
}


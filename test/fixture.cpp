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

class Fixture : public ::testing::Test
{
public:
  static int& val()
  {
    static int _val = 0;
    return _val;
  }
protected:
  void SetUp() override { val() += 10; }
  void TearDown() override { val() -= 1; }
};

TEST_F(Fixture, Run1)
{
  EXPECT_EQ(Fixture::val(), 10);
  Fixture::val() = 0;
  EXPECT_EQ(Fixture::val(), 0);
}

TEST_F(Fixture, Run2)
{
  EXPECT_EQ(Fixture::val(), 9);
}

TEST_F(Fixture, Run3)
{
  EXPECT_EQ(Fixture::val(), 18);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int ret { RUN_ALL_TESTS() };
  
  std::vector<::analysis::Test> expected;
  expected.push_back(::analysis::Test("Fixture", "Run1"));
  expected.push_back(::analysis::Test("Fixture", "Run2"));
  expected.push_back(::analysis::Test("Fixture", "Run3"));
  
  return ::analysis::check(ret, get_stream().str(), expected);
}


#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

template <unsigned idx> bool (*g_launchers())()
{
    return nullptr;
}
namespace testing {
template <std::size_t... I > struct index_sequence {};
template <std::size_t I, std::size_t... Is> struct gen { decltype(gen<I-1, I, Is...>()()) operator() () { return gen<I-1, I, Is...>()(); } };
template <std::size_t... Is> struct gen<0, Is...> { index_sequence<0, Is...> operator() () { index_sequence<0, Is...>(); } };
template <std::size_t I> decltype(gen<I>()()) make_index_sequence() { return gen<I>()(); };
class Test
{
protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};
template <class Runner> bool launch_runner()
{
  std::cout << std::left << "[ " << std::setw(30) << Runner::name() << " ]" << std::endl;
  bool out { Runner().run() };
  const char* status = out ? "FAILED" : "OK";
  std::cout << std::right << "[   " << std::setw(30) << status << " ]" << std::endl;
  return out;
}
constexpr unsigned g_max_num_tests = 200;
void InitGoogleTest(int*, char**) {}
template <unsigned idx> bool RunOneTest()
{
  if (! g_launchers<idx>())
  {
    return 0;
  }
  return g_launchers<idx>()();
}
template <std::size_t... I> int RunTests(::testing::index_sequence<I...>)
{
  bool status[] = { RunOneTest<I>()... };
  bool agg_status = std::find(std::begin(status), std::end(status), true) != std::end(status);
  return agg_status ? 1 : 0;
}
}

#define TEST_IMPLEM(PROJECT_NAME, TEST_NAME, FIXTURE_NAME)                     \
  class Runner__ ## PROJECT_NAME ## __ ## TEST_NAME : public FIXTURE_NAME      \
  {                                                                            \
    bool _has_failed;                                                          \
    void test();                                                               \
  public:                                                                      \
    static std::string name() { return "" #PROJECT_NAME "__" #TEST_NAME ""; }  \
    Runner__ ## PROJECT_NAME ## __ ## TEST_NAME() : _has_failed() {}           \
    bool run() { SetUp(); test(); TearDown(); return _has_failed; }            \
  };                                                                           \
  constexpr unsigned g_runner_id__ ## PROJECT_NAME ## __ ## TEST_NAME = __COUNTER__; \
  template <> bool (*g_launchers<g_runner_id__ ## PROJECT_NAME ## __ ## TEST_NAME>())() \
  {                                                                            \
      return ::testing::launch_runner<Runner__ ## PROJECT_NAME ## __ ## TEST_NAME>; \
  }                                                                            \
  void Runner__ ## PROJECT_NAME ## __ ## TEST_NAME::test()

#define TEST(PROJECT_NAME, TEST_NAME) TEST_IMPLEM(PROJECT_NAME, TEST_NAME, ::testing::Test)
#define TEST_F(FIXTURE_NAME, TEST_NAME) TEST_IMPLEM(FIXTURE_NAME, TEST_NAME, FIXTURE_NAME)

#define EXPECT_IMPLEM(failure, condition, expected, actual) { if (! (condition)) { \
  this->_has_failed = true;                                                    \
  std::cout << std::boolalpha << "\t" << failure << " @ " << __FILE__ << ":" << __LINE__ \
      << "\n\t\tactual: " << actual                                            \
      << "\n\t\texpected: " << expected << std::noboolalpha << std::endl;      \
} }
#define ASSERT_IMPLEM(failure, condition, expected, actual) { if (! (condition)) { \
  EXPECT_IMPLEM(failure, condition, expected, actual);                         \
  return;                                                                      \
} }
#define EXPECT_EQ(expected, val) EXPECT_IMPLEM("EXPECT_EQ", ((expected) == (val)), expected, val)
#define EXPECT_NE(expected, val) EXPECT_IMPLEM("EXPECT_NE", ((expected) != (val)), expected, val)
#define EXPECT_TRUE(val) EXPECT_IMPLEM("EXPECT_TRUE", (!!(val)), true, (!!(val)))
#define EXPECT_FALSE(val) EXPECT_IMPLEM("EXPECT_FALSE", (!(val)), false, (!(val)))
#define ASSERT_EQ(expected, val) ASSERT_IMPLEM("ASSERT_EQ", ((expected) == (val)), expected, val)
#define ASSERT_NE(expected, val) ASSERT_IMPLEM("ASSERT_NE", ((expected) != (val)), expected, val)
#define ASSERT_TRUE(val) ASSERT_IMPLEM("ASSERT_TRUE", (!!(val)), true, (!!(val)))
#define ASSERT_FALSE(val) ASSERT_IMPLEM("ASSERT_FALSE", (!(val)), false, (!(val)))

#define RUN_ALL_TESTS ([]() { return ::testing::RunTests(::testing::make_index_sequence<::testing::g_max_num_tests>()); })

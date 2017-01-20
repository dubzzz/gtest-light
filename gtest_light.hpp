#include <iomanip>
#ifndef TEST_LOGGER
#  include <iostream>
#  define TEST_LOGGER() ::std::cerr
#endif
template <unsigned idx> bool (*g_launchers())() { return nullptr; }
namespace testing {
template <class T1, class T2> void log_error(const char* failure_name, const char* filename, unsigned line, const char* comparison, T1&& v1, T2&& v2) {
  TEST_LOGGER() << ::std::boolalpha << "\t" << failure_name << " @ " << filename << ":" << line
      << "\n\t\texpected: " << v1 << comparison << v2 << ::std::noboolalpha << ::std::endl;
}
template <unsigned... I > struct index_sequence {};
template <unsigned I, unsigned... Is> struct gen { decltype(gen<I-1, I, Is...>()()) operator() () { return gen<I-1, I, Is...>()(); } };
template <unsigned... Is> struct gen<0, Is...> { index_sequence<0, Is...> operator() () { index_sequence<0, Is...>(); } };
template <unsigned I> decltype(gen<I>()()) make_index_sequence() { return gen<I>()(); };
class Test
{
protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};
template <class Runner> bool launch_runner()
{
  TEST_LOGGER() << ::std::left << "[ " << ::std::setw(30) << Runner::name << " ]" << ::std::endl;
  bool out { Runner().run() };
  const char* status = out ? "FAILED" : "OK";
  TEST_LOGGER() << ::std::right << "[ " << ::std::setw(30) << status << " ]" << ::std::endl;
  return out;
}
constexpr unsigned g_max_num_tests = 200;
void InitGoogleTest(int*, char**) {}
template <unsigned idx> bool RunOneTest() { return !!g_launchers<idx>() ? g_launchers<idx>()() : 0; }
template <unsigned... I> int RunTests(::testing::index_sequence<I...>)
{
  for (auto s : { RunOneTest<I>()... }) { if (s) return 1; }
  return 0;
}
}

#define TEST_IMPLEM(PROJECT_NAME, TEST_NAME, FIXTURE_NAME)                     \
  class Runner__ ## PROJECT_NAME ## __ ## TEST_NAME : public FIXTURE_NAME      \
  {                                                                            \
    bool _has_failed;                                                          \
    void test();                                                               \
  public:                                                                      \
    static const char name[];                                                  \
    Runner__ ## PROJECT_NAME ## __ ## TEST_NAME() : _has_failed() {}           \
    bool run() { SetUp(); test(); TearDown(); return _has_failed; }            \
  };                                                                           \
  const char Runner__ ## PROJECT_NAME ## __ ## TEST_NAME ::name[]              \
      = "" #PROJECT_NAME "__" #TEST_NAME "";                                   \
  constexpr unsigned g_runner_id__ ## PROJECT_NAME ## __ ## TEST_NAME = __COUNTER__; \
  template <> bool (*g_launchers<g_runner_id__ ## PROJECT_NAME ## __ ## TEST_NAME>())() \
  {                                                                            \
    return ::testing::launch_runner<Runner__ ## PROJECT_NAME ## __ ## TEST_NAME>; \
  }                                                                            \
  void Runner__ ## PROJECT_NAME ## __ ## TEST_NAME::test()

#define TEST(PROJECT_NAME, TEST_NAME) TEST_IMPLEM(PROJECT_NAME, TEST_NAME, ::testing::Test)
#define TEST_F(FIXTURE_NAME, TEST_NAME) TEST_IMPLEM(FIXTURE_NAME, TEST_NAME, FIXTURE_NAME)

#define EXPECT_IMPLEM(failure, condition, comparison, expected, actual) { if (! (condition)) { \
  this->_has_failed = true;                                                    \
  ::testing::log_error(failure, __FILE__, __LINE__, comparison, actual, expected); \
} }
#define ASSERT_IMPLEM(failure, condition, comparison, expected, actual) { if (! (condition)) { \
  EXPECT_IMPLEM(failure, condition, comparison, expected, actual);                         \
  return;                                                                      \
} }
#define EXPECT_EQ(expected, val) EXPECT_IMPLEM("EXPECT_EQ", ((expected) == (val)), " == ", expected, val)
#define EXPECT_NE(expected, val) EXPECT_IMPLEM("EXPECT_NE", ((expected) != (val)), " != ", expected, val)
#define EXPECT_TRUE(val) EXPECT_IMPLEM("EXPECT_TRUE", (!!(val)), " == ", true, (!!(val)))
#define EXPECT_FALSE(val) EXPECT_IMPLEM("EXPECT_FALSE", (!(val)), " == ", false, (!(val)))
#define ASSERT_EQ(expected, val) ASSERT_IMPLEM("ASSERT_EQ", ((expected) == (val)), " == ", expected, val)
#define ASSERT_NE(expected, val) ASSERT_IMPLEM("ASSERT_NE", ((expected) != (val)), " != ", expected, val)
#define ASSERT_TRUE(val) ASSERT_IMPLEM("ASSERT_TRUE", (!!(val)), " == ", true, (!!(val)))
#define ASSERT_FALSE(val) ASSERT_IMPLEM("ASSERT_FALSE", (!(val)), " == ", false, (!(val)))

#define RUN_ALL_TESTS ([]() { return ::testing::RunTests(::testing::make_index_sequence<::testing::g_max_num_tests>()); })

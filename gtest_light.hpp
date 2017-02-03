#include <iomanip>
#include <iterator>
#include <type_traits>
#include <typeinfo>
#ifndef TEST_LOGGER
#  include <iostream>
#  define TEST_LOGGER() ::std::cerr
#endif
template <unsigned idx> bool (*g_launchers())() { return nullptr; }
namespace testing {
template <class... Ts> struct make_void { using type = void; };            
template <class... Ts> using void_t = typename make_void<Ts...>::type;
template <class T, class = void> struct PrintAll {
  using InternalT = typename ::std::decay<T>::type const&;
  InternalT _t;
  PrintAll(InternalT t) : _t(t) {}
  ::std::ostream& print(::std::ostream& os) const { return (os << "<instance of " << typeid(_t).name() << ">"); }
};
template <class T> struct PrintAll<T, void_t<decltype(TEST_LOGGER() << ::std::declval<T>())>>{
  using InternalT = typename ::std::decay<T>::type const&;
  InternalT _t;
  PrintAll(InternalT t) : _t(t) {}
  ::std::ostream& print(::std::ostream& os) const { return (os << _t); }
};
template <class Type> ::std::ostream& operator<<(::std::ostream& os, PrintAll<Type, void> const& t) { return t.print(os); }
template <class T1, class T2> void log_error(const char* failure_name, const char* filename, unsigned line, const char* comparison, T1&& v1, T2&& v2) {
  TEST_LOGGER() << ::std::boolalpha << "  " << failure_name << "\n    " << filename << ":" << line << "\n    expected: "
      << ::testing::PrintAll<T1>(v1) << comparison
      << ::testing::PrintAll<T2>(v2) << ::std::noboolalpha << ::std::endl;
}
struct TestEq {
  static const char sign[];
  template <class T1,  class T2> bool operator()(T1&& t1, T2&& t2) const { return t1 == t2; } };
struct TestNe {
  static const char sign[];
  template <class T1,  class T2> bool operator()(T1&& t1, T2&& t2) const { return t1 != t2; } };
struct TestTrue {
  static const char sign[];
  template <class T1,  class T2> bool operator()(T1&& t1, T2&&) const { return !!t1; } };
struct TestFalse {
  static const char sign[];
  template <class T1,  class T2> bool operator()(T1&& t1, T2&&) const { return !t1; } };
const char TestEq::sign[] = " == ";
const char TestNe::sign[] = " != ";
const char TestTrue::sign[] = " == ";
const char TestFalse::sign[] = " == ";
template <class Comp, class T1, class T2> bool expect_impl(const char* failure, const char* file, unsigned line, T1&& t1, T2&& t2) {
  if (! Comp()(t1, t2)) {
    ::testing::log_error(failure, file, line, Comp::sign, t1, t2);
    return true;
  }
  return false;
}
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
template <unsigned idx> bool RunOneTest() { return !!g_launchers<idx>() ? g_launchers<idx>()() : false; }
template <unsigned idx = 0> int RunTests() { return (RunOneTest<idx>() ? 1 : 0) + RunTests<idx+1>(); }
template <> int RunTests<::testing::g_max_num_tests>() { return 0; }
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

#define EXPECT_IMPLEM(failure, CompClass, expected, actual) { this->_has_failed |= ::testing::expect_impl<CompClass>(failure, __FILE__, __LINE__, (actual), (expected)); }
#define ASSERT_IMPLEM(failure, CompClass, expected, actual) { if (::testing::expect_impl<CompClass>(failure, __FILE__, __LINE__, (actual), (expected))) { \
  this->_has_failed = true;                                                    \
  return;                                                                      \
} }
#define EXPECT_EQ(expected, val) EXPECT_IMPLEM("EXPECT_EQ", ::testing::TestEq, expected, val)
#define EXPECT_NE(expected, val) EXPECT_IMPLEM("EXPECT_NE", ::testing::TestNe, expected, val)
#define EXPECT_TRUE(val) EXPECT_IMPLEM("EXPECT_TRUE", ::testing::TestTrue, true, val)
#define EXPECT_FALSE(val) EXPECT_IMPLEM("EXPECT_FALSE", ::testing::TestFalse, false, val)
#define ASSERT_EQ(expected, val) ASSERT_IMPLEM("ASSERT_EQ", ::testing::TestEq, expected, val)
#define ASSERT_NE(expected, val) ASSERT_IMPLEM("ASSERT_NE", ::testing::TestNe, expected, val)
#define ASSERT_TRUE(val) ASSERT_IMPLEM("ASSERT_TRUE", ::testing::TestTrue, true, val)
#define ASSERT_FALSE(val) ASSERT_IMPLEM("ASSERT_FALSE", ::testing::TestFalse, false, val)

#define RUN_ALL_TESTS ([]() { return ::testing::RunTests() != 0 ? 1 : 0; })

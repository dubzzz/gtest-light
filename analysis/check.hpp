#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <vector>

namespace analysis {

enum class Level { Assert, Expect };
enum class Operation { Eq, Ne, True, False };

class Test
{
  const std::string _project_name;
  const std::string _name;
  std::map<int, std::tuple<::analysis::Level, ::analysis::Operation, std::string, std::string>> _failures;

public:
  Test(std::string const& project_name, std::string const& name)
      : _project_name(project_name)
      , _name(name)
      , _failures()
  {}
  std::string const& project_name() const { return _project_name; }
  std::string const& name() const { return _name; }
  std::map<int, std::tuple<::analysis::Level, ::analysis::Operation, std::string, std::string>> const& failures() const { return _failures; }
  Test& withFailure(int line, ::analysis::Level level, ::analysis::Operation op, std::string const& v1, std::string const v2)
  {
    _failures.insert(std::make_pair(line, std::make_tuple(level, op, v1, v2)));
    return *this;
  }
};

template <class... Ts> using void_t = void;
template <class T, class = void> struct can_be_printed : std::false_type {};
template <class T> struct can_be_printed<T, void_t<decltype(::std::cout << ::std::declval<T>())>> : std::true_type {};

template <class T> std::string to_string(T&& val, typename std::enable_if<can_be_printed<T>::value>::type* = nullptr)
{
  std::ostringstream oss;
  oss << val;
  return oss.str();
}
template <class T> std::string to_string(T&& val, typename std::enable_if<! can_be_printed<T>::value>::type* = nullptr)
{
  std::ostringstream oss;
  oss << "<instance of " << typeid(val).name() << ">";
  return oss.str();
}

int check(int /*retcode*/, std::string const& /*output*/, std::vector<::analysis::Test> const& /*details*/);

} //analysis


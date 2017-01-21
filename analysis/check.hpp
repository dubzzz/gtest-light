#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace analysis {

enum class Level { Assert, Expect };
enum class Operation { Eq };

class Test
{
  const std::string _project_name;
  const std::string _name;
  std::map<int, std::tuple<Level, Operation, std::string, std::string>> _failures;

public:
  Test(std::string const& project_name, std::string const& name)
      : _project_name(project_name)
      , _name(name)
      , _failures()
  {}
  std::string const& project_name() const { return _project_name; }
  std::string const& name() const { return _name; }
  std::map<int, std::tuple<Level, Operation, std::string, std::string>> const& failures() const { return _failures; }
  Test& withFailure(int line, Level level, Operation op, std::string const& v1, std::string const v2)
  {
    _failures.insert(std::make_pair(line, std::make_tuple(level, op, v1, v2)));
    return *this;
  }
};

int check(int /*retcode*/, std::string const& /*output*/, std::vector<Test> const& /*details*/)
{
  return 0;
}

} //analysis


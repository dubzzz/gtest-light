#include <map>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

#include "check.hpp"

namespace analysis {

std::string escape_for_regex(std::string const& origin)
{
  std::string dest {};
  for (auto&& c : origin)
  {
    // to be escaped: ^ $ \ . * + ? ( ) [ ] { } |
    if (c == '^' || c == '$' || c == '\\' || c == '.' || c == '*' || c == '+' || c == '?' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '|')
    {
      dest += '\\';
    }
    dest += c;
  }
  return dest;
}
std::string to_failure_name(Level level, Operation op)
{
  std::string name {};
  switch (level)
  {
    case Level::Assert: name += "ASSERT"; break;
    case Level::Expect: name += "EXPECT"; break;
  }
  name += '_';
  switch (op)
  {
    case Operation::Eq: name += "EQ"; break;
  }
  return name;
}
std::string to_op_repr(Operation op)
{
  switch (op)
  {
    case Operation::Eq: return " == ";
  }
  return "";
}

void push_header_pattern(std::vector<std::string>& expected, Test const& test)
{
  expected.emplace_back("^\\[ "
      + escape_for_regex(test.project_name() + "__" + test.name())
      + "\\s*\\]$");
}
void push_failure_pattern(std::vector<std::string>& expected, std::pair<int, std::tuple<Level, Operation, std::string, std::string>> const& failure)
{
  int line = failure.first;
  Level const& level = std::get<0>(failure.second);
  Operation const& op = std::get<1>(failure.second);
  std::string const& v1 = std::get<2>(failure.second);
  std::string const& v2 = std::get<3>(failure.second);

  expected.emplace_back("^\\s+" + escape_for_regex(to_failure_name(level, op)) + "$");
  expected.emplace_back("^\\s+.*:" + std::to_string(line) + "$");
  expected.emplace_back("^\\s+" + escape_for_regex(v1 + to_op_repr(op) + v2) + "$");
}
void push_body_pattern(std::vector<std::string>& expected, Test const& test)
{
  for (auto&& failure : test.failures())
  {
    push_failure_pattern(expected, failure);
  }
}
void push_footer_pattern(std::vector<std::string>& expected, Test const& test)
{
  expected.emplace_back(test.failures().empty()
      ? "^\\[ \\s+OK \\]$"
      : "^\\[ \\s+FAILED \\]$");
}

std::vector<std::string> build_patterns(std::vector<Test> const& details)
{
  std::vector<std::string> expected_patterns;
  for (auto&& test : details)
  {
    push_header_pattern(expected_patterns, test);
    push_body_pattern(expected_patterns, test);
    push_footer_pattern(expected_patterns, test);
  }
  return expected_patterns;
}
int compute_retcode(std::vector<Test> const& details)
{
  auto&& it_with_failures = std::find_if(details.begin(), details.end(), [](decltype(details[0]) const& test) { return ! test.failures().empty(); });
  return it_with_failures != details.end() ? 1 : 0;
}

std::vector<std::string> split_string(std::string const& str, std::string const& delimiter)
{
  std::vector<std::string> strings;
  std::string::size_type pos {};
  std::string::size_type prev {};
  while ((pos = str.find(delimiter, prev)) != std::string::npos)
  {
    strings.push_back(str.substr(prev, pos - prev));
    prev = pos + 1;
  }
  strings.push_back(str.substr(prev));
  return strings;
}

int check(int retcode, std::string const& output, std::vector<Test> const& details)
{
  std::vector<std::string> expected_patterns { build_patterns(details) };
  int expected_retcode { compute_retcode(details) };

  std::vector<std::string> output_lines { split_string(output, "\n") };

  bool failed {};
  std::size_t num { std::max(output_lines.size(), expected_patterns.size()) };
  for (std::size_t idx {} ; idx != num ; ++idx)
  {
    //TODO
  }
  failed |= (retcode != expected_retcode);

  return failed ? 1 : 0;
}

} //analysis


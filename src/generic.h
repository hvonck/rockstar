#pragma once
#include <vector>
#include <string>

namespace lambda
{
  extern const std::vector<std::string> k_true_aliases;
  extern const std::vector<std::string> k_false_aliases;
  extern const std::vector<std::string> k_null_aliases;
  extern const std::vector<std::string> k_add_aliases;
  extern const std::vector<std::string> k_subtract_aliases;
  extern const std::vector<std::string> k_multiply_aliases;
  extern const std::vector<std::string> k_divide_aliases;
  extern const std::vector<std::string> k_it_aliases;
  extern const std::vector<std::string> k_common_aliases_;

  extern const std::vector<std::string> k_say_aliases;
  extern const std::vector<std::string> k_is_aliases;
  extern const std::vector<std::string> k_greater_than_aliases;
  extern const std::vector<std::string> k_less_than_aliases;
  extern const std::vector<std::string> k_greater_than_or_equal_to_aliases;
  extern const std::vector<std::string> k_less_than_or_equal_to_aliases;

  extern std::string cleanup(const std::string& str);
  extern std::string toLower(const std::string& str);
  extern std::vector<std::string> toLower(const std::vector<std::string>& vec);
  extern std::string replaceAll(const std::string& str, const std::string& find, const std::string& replace);
  extern std::vector<std::string> split(const std::string& str, const char& delim);
  extern std::vector<std::string> split(const std::string& str, const std::string& delim);
  extern float stringToReal(const std::string& str);
  extern bool isLower(const std::string& str);
  extern bool isUpper(const std::string& str);
  extern bool contains(const std::string& lhs, const std::string& rhs);
  extern bool contains(const std::vector<std::string>& lhs, const std::string& rhs);
  extern bool contains(const std::vector<std::string>& lhs, const std::vector<std::string>& rhs);
  extern bool contains(const std::string& lhs, const std::vector<std::string>& rhs);
  extern bool startsWith(const std::string& lhs, const std::string& rhs);
  extern bool startsWith(const std::vector<std::string>& lhs, const std::string& rhs);
  extern bool startsWith(const std::vector<std::string>& lhs, const std::vector<std::string>& rhs);
  extern bool startsWith(const std::string& lhs, const std::vector<std::string>& rhs);
  extern bool equals(const std::string& lhs, const std::string& rhs);
  extern bool equals(const std::vector<std::string>& lhs, const std::string& rhs);
  extern bool equals(const std::vector<std::string>& lhs, const std::vector<std::string>& rhs);
  extern bool equals(const std::string& lhs, const std::vector<std::string>& rhs);
  extern std::string vecToStr(const std::vector<std::string>& v);
  extern bool isStrReal(const std::string& s);
}
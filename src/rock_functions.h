#pragma once
#include "containers.h"

namespace lambda
{
  extern bool rockIsFloat(const String& word, float* f = nullptr);
  extern bool rockIsBool(const String& word, bool* b = nullptr);
  extern bool rockEqual(const char& lhs, const char& rhs);
  extern bool rockEqual(const char& lhs, const Vector<char>& rhs);
  extern bool rockEqual(const Vector<char>& lhs, const Vector<char>& rhs);
  extern bool rockEqual(const String& lhs, const String& rhs);
  extern bool rockEqual(const String& lhs, const Vector<String>& rhs);
  extern bool rockEqual(const Vector<String>& lhs, const Vector<String>& rhs);
  extern Vector<String> rockSplit(const String& line, const char& delim);
  extern Vector<String> rockSplit(const Vector<String>& line, const String& delim);
  extern String rockToString(const Vector<String>& line, const size_t& start = 0u, const size_t& end = 0u);
  extern String rockToString(size_t t);
  extern String rockToLower(const String& word);
  extern float rockStringToNumber(const String& line);
}
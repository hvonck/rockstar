#include "rock_functions.h"
#include <sstream>

namespace lambda
{
  bool rockIsFloat(const String& word, float* f)
  {
    std::istringstream iss(word);
    if (f == nullptr)
    {
      float dummy;
      iss >> std::noskipws >> dummy;
    }
    else
    {
      iss >> std::noskipws >> *f;
    }
    return iss && iss.eof();     // Result converted to bool
  }
  bool rockIsBool(const String& word, bool* b)
  {
    // TODO (Hilze): Do something about this?
    Vector<String> true_ = {
      "true",
      "right",
      "yes",
      "ok"
    };
    Vector<String> false_ = {
      "false",
      "wrong",
      "no",
      "lies"
    };

    for (const String& v : true_)
    {
      if (word == v)
      {
        if (b != nullptr) *b = true;
        return true;
      }
    }
    for (const String& v : false_)
    {
      if (word == v)
      {
        if (b != nullptr) *b = false;
        return true;
      }
    }
    return false;
  }
  bool rockEqual(const String& lhs, const String& rhs)
  {
    if (lhs.size() != rhs.size())
    {
      return false;
    }
    else
    {
      return lhs == rhs;
    }
  }
  bool rockEqual(const String& lhs, const Vector<String>& rhs)
  {
    for (const String& type : rhs)
    {
      if (rockEqual(lhs, type))
      {
        return true;
      }
    }
    return false;
  }
  bool rockEqual(const Vector<String>& lhs, const Vector<String>& rhs)
  {
    for (const String& word : lhs)
    {
      if (rockEqual(word, rhs))
      {
        return true;
      }
    }
    return false;
  }
    
  Vector<String> rockSplit(const String& line, const char& delim)
  {
    std::stringstream ss(line.c_str());
    Vector<String> lines;

    String to;
    while (std::getline(ss, to, delim))
    {
      if (to.empty() == false)
      {
        lines.push_back(to);
      }
    }

    return std::move(lines);
  }
  String rockToString(const Vector<String>& line, const size_t& start, const size_t& end)
  {
    String ret;
    for (size_t i = start; i < (end == 0 ? line.size() : end); ++i)
    {
      ret += line.at(i) + " ";
    }
    ret.pop_back();
    return ret;
  }
  float rockStringToNumber(const String& line)
  {
    float number  = 0.0f;
    float decimal = 0.0f;

    for (const String& word : rockSplit(line, ' '))
    {
      number  *= 10u;
      decimal *= 10u;

      float n = 0.0f;

      for (const char& ch : word)
      {
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        {
          n++;
        }
        else if (ch == '.' && decimal == 0.0f)
        {
          decimal = 1.0f;
        }
      }
      number += std::fmodf(n, 10.0f);
    }

    if (decimal == 0.0f)
    {
      return number;
    }
    else
    {
      return number / decimal;
    }
  }
}
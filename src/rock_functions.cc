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
  bool rockEqual(const char& lhs, const char& rhs)
  {
    return lhs == rhs;
  }
  bool rockEqual(const char& lhs, const Vector<char>& rhs)
  {
    for (const char& ch : rhs)
    {
      if (rockEqual(lhs, ch))
      {
        return true;
      }
    }
    return false;
  }
  bool rockEqual(const Vector<char>& lhs, const Vector<char>& rhs)
  {
    for (const char& ch : lhs)
    {
      if (rockEqual(ch, rhs))
      {
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
      lines.push_back(to);
    }

    return std::move(lines);
  }
  Vector<String> rockSplit(const Vector<String>& line, const String& delim)
  {
    Vector<String> segments;
    String segment;

    for (const String& word : line)
    {
      if (rockEqual(word, delim))
      {
        if (false == segment.empty())
        {
          segments.push_back(segment);
          segment.clear();
        }
      }
      else
      {
        if (false == segment.empty())
        {
          segment += " " + word;
        }
        else
        {
          segment += word;
        }
      }
    }
    if (false == segment.empty())
    {
      segments.push_back(segment);
    }
    return segments;
  }
  String rockToString(const Vector<String>& line, const size_t& start, const size_t& end)
  {
    String ret;
    for (size_t i = start; i < (end == 0 ? line.size() : end); ++i)
    {
      ret += line.at(i) + " ";
    }
    if (false == ret.empty())
    {
      ret.pop_back();
    }
    return ret;
  }
  String rockToString(size_t t)
  {
    // Not quick, but works with both the standard library and eastl.
    if (t == 0u)
    {
      return "0";
    }

    String str;

    while (t > 0u)
    {
      str += (t % 10) + '0';
      t /= 10;
    }

    std::reverse(str.begin(), str.end());

    return str;
  }
  String rockToLower(const String& word)
  {
    String str = word;
    for (char& ch : str)
    {
      ch = tolower(ch);
    }
    return str;
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
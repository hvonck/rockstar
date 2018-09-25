#include "generic.h"
#include <sstream>

namespace lambda
{
  const std::vector<std::string> k_true_aliases = {
    "true",
    "right",
    "yes",
    "ok"
  };
  const std::vector<std::string> k_false_aliases = {
    "false",
    "wrong",
    "no",
    "lies"
  };
  const std::vector<std::string> k_null_aliases = {
    "null",
    "nothing",
    "nowhere",
    "nobody",
    "empty",
    "gone"
  };
  const std::vector<std::string> k_add_aliases = {
    "plus",
    "with"
  };
  const std::vector<std::string> k_subtract_aliases = {
    "minus",
    "without"
  };
  const std::vector<std::string> k_multiply_aliases = {
    "times",
    "of"
  };
  const std::vector<std::string> k_divide_aliases = {
    "over"
  };
  const std::vector<std::string> k_it_aliases = {
    "it",
    "he",
    "she",
    "him",
    "her",
    "they",
    "them",
    "ze",
    "hir",
    "zie",
    "zir",
    "xe",
    "xem",
    "ve",
    "ver",
  };
  const std::vector<std::string> k_common_aliases_ = {
    "a ",
    "an ",
    "the ",
    "my ",
    "your "
  };

  const std::vector<std::string> k_say_aliases = {
    "say",
    "shout",
    "whisper",
    "scream"
  };
  const std::vector<std::string> k_is_aliases = {
    "is",
    "was",
    "were"
  };
  const std::vector<std::string> k_greater_than_aliases = {
    "higher",
    "greater",
    "bigger",
    "stronger"
  };
  const std::vector<std::string> k_less_than_aliases = {
    "lower",
    "less",
    "smaller",
    "weaker"
  };
  const std::vector<std::string> k_greater_than_or_equal_to_aliases = {
    "high",
    "great",
    "big",
    "strong"
  };
  const std::vector<std::string> k_less_than_or_equal_to_aliases = {
    "low",
    "little",
    "small",
    "weak"
  };

  std::string cleanup(const std::string& str)
  {
    std::string l = str;
    if (!l.empty() && l.at(0u) == ' ')
    {
      l.erase(l.begin(), l.begin() + l.find_first_not_of(' '));
    }
    if (!l.empty() && l.find_first_of('(') != std::string::npos)
    {
      l.erase(l.begin() + l.find_first_of('('), l.end());
    }
    if (!l.empty() && l.find_last_not_of(' ') != l.size() - 1u)
    {
      l.erase(l.begin() + l.find_last_not_of(' ') + 1u, l.end());
    }
    return l;
  }

  std::string toLower(const std::string& str)
  {
    std::string s = str;
    for (char& ch : s) ch = tolower(ch);
    return s;
  }

  std::vector<std::string> toLower(const std::vector<std::string>& vec)
  {
    std::vector<std::string> ret;
    for (std::string s : vec)
    {
      ret.push_back(toLower(s));
    }
    return ret;
  }

  std::string replaceAll(const std::string & str, const std::string & find, const std::string & replace)
  {
    std::string s = str;

    // Get the first occurrence
    size_t pos = s.find(find);

    // Repeat till end is reached
    while (pos != std::string::npos)
    {
      // Replace this occurrence of Sub String
      s.replace(pos, find.size(), replace);
      // Get the next occurrence from the current position
      pos = s.find(find, pos + replace.size());
    }

    return s;
  }

  std::vector<std::string> split(const std::string& str, const char& delim)
  {
    std::stringstream ss(str);
    std::vector<std::string> v;

    std::string to;
    while (std::getline(ss, to, delim))
    {
      v.push_back(to);
    }

    return std::move(v);
  }
  std::vector<std::string> split(const std::string& str, const std::string& delim)
  {
    std::string s = str;
    std::vector<std::string> segments;
    
    size_t pos = 0u;
    while ((pos = s.find(delim)) != std::string::npos) 
    {
      segments.push_back(s.substr(0u, pos));
      s.erase(0, pos + delim.length());
    }
    if (false == s.empty())
    {
      segments.push_back(s);
    }

    return std::move(segments);
  }

  float stringToReal(const std::string& str)
  {
    float number  = 0.0f;
    float decimal = 0.0f;

    for (const std::string& word : split(str, ' '))
    {
      number  *= 10.0f;
      decimal *= 10.0f;

      float n = 0.0f;

      for (const char& ch : word)
      {
        if (ch == '.' && decimal == 0.0f)
        {
          decimal = 1.0f;
        }
        else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
          n++;
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

  bool isLower(const std::string& str)
  {
    for (const char& ch : str)
    {
      if (ch >= 'A' && ch <= 'Z')
      {
        return false;
      }
    }
    return true;
  }

  bool isUpper(const std::string& str)
  {
    for (const char& ch : str)
    {
      if (!isupper(ch))
      {
        return false;
      }
    }
    return true;
  }

  bool contains(const std::string& lhs, const std::string& rhs)
  {
    return lhs.find(rhs) != std::string::npos;
  }
  bool contains(const std::vector<std::string>& lhs, const std::string& rhs)
  {
    for (const std::string& l : lhs)
    {
      if (true == contains(l, rhs))
      {
        return true;
      }
    }

    return false;
  }
  bool contains(const std::vector<std::string>& lhs, const std::vector<std::string>& rhs)
  {
    for (const std::string& l : rhs)
    {
      if (true == contains(lhs, l))
      {
        return true;
      }
    }

    return false;
  }
  bool contains(const std::string& lhs, const std::vector<std::string>& rhs)
  {
    for (const std::string& l : rhs)
    {
      if (true == contains(lhs, l))
      {
        return true;
      }
    }

    return false;
  }
  bool startsWith(const std::string& lhs, const std::string& rhs)
  {
    return (lhs.find(rhs) == 0u);
  }
  bool startsWith(const std::vector<std::string>& lhs, const std::string& rhs)
  {
    for (const std::string& str : lhs)
    {
      if (startsWith(str, rhs))
      {
        return true;
      }
    }
    return false;
  }
  bool startsWith(const std::vector<std::string>& lhs, const std::vector<std::string>& rhs)
  {
    for (const std::string& str : lhs)
    {
      if (startsWith(str, rhs))
      {
        return true;
      }
    }
    return false;
  }
  bool startsWith(const std::string& lhs, const std::vector<std::string>& rhs)
  {
    for (const std::string& str : rhs)
    {
      if (startsWith(lhs, str))
      {
        return true;
      }
    }
    return false;
  }
  bool equals(const std::string& lhs, const std::string& rhs)
  {
    return lhs.size() == rhs.size() && lhs == rhs;
  }
  bool equals(const std::vector<std::string>& lhs, const std::string& rhs)
  {
    for (const std::string& l : lhs)
    {
      if (true == equals(l, rhs))
      {
        return true;
      }
    }

    return false;
  }
  bool equals(const std::vector<std::string>& lhs, const std::vector<std::string>& rhs)
  {
    for (const std::string& l : rhs)
    {
      if (true == equals(lhs, l))
      {
        return true;
      }
    }

    return false;
  }
  bool equals(const std::string& lhs, const std::vector<std::string>& rhs)
  {
    for (const std::string& l : rhs)
    {
      if (true == equals(lhs, l))
      {
        return true;
      }
    }

    return false;
  }
  std::string vecToStr(const std::vector<std::string>& v)
  {
    std::string s;
    for (const std::string& l : v)
    {
      s += l + " ";
    }

    if (!s.empty())
    {
      s.pop_back();
    }

    return s;
  }
  bool isStrReal(const std::string& s)
  {
    std::istringstream iss(s);
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
  }
}
#pragma once
#include <string>
#include <vector>

namespace lambda
{
  enum VarType
  {
    kVarReal,
    kVarString,
    kVarBool,
    kVarUnknown
  };

  inline std::string typeToStr(const VarType& type)
  {
    switch (type)
    {
    case kVarReal:
      return "real";
    case kVarString:
      return "string";
    case kVarBool:
      return "bool";
    case kVarUnknown:
      return "mistery";
    }
    return "unknown";
  }

  class Var
  {
  public:
    void make(const float& v);
    void make(const std::string& v);
    void make(const bool&  v);
    void make(const char* v);
    void make(const Var* v);
    VarType getType() const;

    float       asReal()   const;
    std::string asString() const;
    bool        asBool()   const;

    bool convert(const VarType& t);
    void make(const std::vector<char>& data, const VarType& type);

  private:
    void make(void* mem, size_t size);

  private:
    std::vector<char> data_;
    VarType type_ = kVarUnknown;
  };
}
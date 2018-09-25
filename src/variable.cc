#pragma once
#include "variable.h"
#include "generic.h"
#include <memory>

namespace lambda
{
  void Var::make(const float& v) 
  { 
    make((void*)&v, sizeof(v));
    type_ = kVarReal; 
  }
  void Var::make(const std::string& v) 
  { 
    make((void*)v.data(), v.size() + 1u); 
    type_ = kVarString; 
  }
  void Var::make(const bool&  v)
  {
    make((void*)&v, sizeof(v));   
    type_ = kVarBool; 
  }
  void Var::make(const char* v) 
  { 
    make(std::string(v)); 
  }
  void Var::make(const Var* v)
  {
    data_ = v->data_;
    type_ = v->type_;
  }
  VarType Var::getType() const 
  {
    return type_; 
  }

  float Var::asReal()  const
  { 
    return *(float*)data_.data();
  }
  std::string Var::asString() const
  { 
    return std::string((char*)data_.data());
  }
  bool Var::asBool() const
  { 
    return *(bool*)data_.data();
  }

  bool Var::convert(const VarType& t)
  {
    if (type_ == t)
    {
      return true;
    }

    switch (type_)
    {
    case kVarReal:
      switch (t)
      {
      case kVarString:
        make(std::to_string(asReal()));
        return true;
      case kVarBool:
        make(asReal() > 0.0f ? true : false);
        return true;
      case kVarUnknown:
        make(0.0f);
        return true;
      }
      break;
    case kVarString:
      switch (t)
      {
      case kVarReal:
        make(stringToReal(asString()));
        return true;
      case kVarBool:
        if (equals(asString(), k_false_aliases)) make(false);
        else if (equals(asString(), k_true_aliases)) make(true);
        else make(asString().size() > 0u ? true : false);
        return true;
      case kVarUnknown:
        make(0.0f);
        return true;
      }
      break;
    case kVarBool:
      switch (t)
      {
      case kVarReal:
        make((float)asBool());
        return true;
      case kVarString:
        if (asBool() == false) make("false");
        else make("true");
        return true;
      case kVarUnknown:
        make(0.0f);
        return true;
      }
      break;
    case kVarUnknown:
      switch (t)
      {
      case kVarReal:
        make(0.0f);
        return true;
      case kVarString:
        make("");
        return true;
      case kVarBool:
        make(false);
        return true;
      }
      break;
    }
    return false;
  }

  void Var::make(const std::vector<char>& data, const VarType& type)
  {
    make((void*)data.data(), data.size());
    type_ = type;
  }

  void Var::make(void* mem, size_t size)
  {
    data_.resize(size);
    
    memcpy(data_.data(), mem, size);
  }
}
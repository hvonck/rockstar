#pragma once
#include "containers.h"

namespace lambda
{
  enum class Types
  {
    kMysterious,
    kNull,
    kBool,
    kNumber,
    kString,
    kObject,
    kFunction,
    kCount
  };

  inline String typeToString(const Types& type)
  {
    switch (type)
    {
    default:
    case Types::kMysterious: return "misterious";
    case Types::kNull:       return "null";
    case Types::kBool:       return "bool";
    case Types::kNumber:     return "number";
    case Types::kString:     return "string";
    case Types::kObject:     return "object";
    case Types::kFunction:   return "function";
    }
  }

  class IType;
  class MysteriousType;
  class NullType;
  class BoolType;
  class NumberType;
  class StringType;
  class ObjectType;
  class FunctionType;

  class TypeContainer
  {
  public:
    void setName(const String& name) { name_ = name; }
    void construct(const String& name, MysteriousType* data) { name_ = name; data_ = (IType*)data; type_ = Types::kMysterious; }
    void construct(const String& name, NullType* data)       { name_ = name; data_ = (IType*)data; type_ = Types::kNull;       }
    void construct(const String& name, BoolType* data)       { name_ = name; data_ = (IType*)data; type_ = Types::kBool;       }
    void construct(const String& name, NumberType* data)     { name_ = name; data_ = (IType*)data; type_ = Types::kNumber;     }
    void construct(const String& name, StringType* data)     { name_ = name; data_ = (IType*)data; type_ = Types::kString;     }
    void construct(const String& name, ObjectType* data)     { name_ = name; data_ = (IType*)data; type_ = Types::kObject;     }
    void construct(const String& name, FunctionType* data)   { name_ = name; data_ = (IType*)data; type_ = Types::kFunction;   }
    String          getName()      const { return name_;                  }
    Types           getType()      const { return type_;                  }
    MysteriousType* asMysterious() const { return (MysteriousType*)data_; }
    NullType*       asNull()       const { return (NullType*)data_;       }
    BoolType*       asBool()       const { return (BoolType*)data_;       }
    NumberType*     asNumber()     const { return (NumberType*)data_;     }
    StringType*     asString()     const { return (StringType*)data_;     }
    ObjectType*     asObject()     const { return (ObjectType*)data_;     }
    FunctionType*   asFunction()   const { return (FunctionType*)data_;   }

  private:
    String name_ = "";
    Types  type_ = Types::kMysterious;
    IType* data_ = nullptr;
  };

  class IType
  {
  };
  class MysteriousType : public IType
  {
  public:
    void* getMysterious() const { return mysterious_; }

  private:
    void* mysterious_ = nullptr;
  };
  class NullType : public IType
  {
  public:
    void* getNull() const { return null; }

  private:
    void* null = nullptr;
  };
  class BoolType : public IType
  {
  public:
    bool get() const { return boolean_; }
    void set(const bool& boolean) { boolean_ = boolean; }

  private:
    bool boolean_ = false;
  };
  class NumberType : public IType
  {
  public:
    void set(const float& number) { number_ = number; }
    float get() const { return number_; }

  private:
    float number_ = 0.0f;
  };
  class StringType : public IType
  {
  public:
    String get() const { return string_; }
    void set(const String& string) { string_ = string; }

  private:
    String string_;
  };
  class ObjectType : public IType
  {
  public:
    void* get() const { return object_; }
    void set(void* object) { object_ = object; }

  private:
    void* object_ = nullptr;
  };
  class FunctionType : public IType
  {
  public:
    void* get() const { return function_; }
    void set(void* function) { function_ = function_; }

  private:
    void* function_ = nullptr;
  };
}
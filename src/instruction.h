#pragma once
#include "variable.h"
#include <string>
#include <iostream>
#include "stack.h"
#include "scope.h"
#include "generic.h"

namespace lambda
{
  class IInstruction
  {
  public:
    virtual bool canExec() const { return true; }
    virtual bool exec(Stack* stack) const { return true; };
    std::string errorMessage() const { return error_; };

  protected:
    std::string error_;
    void setError(const std::string& error) const { ((IInstruction*)this)->error_ = error; }
  };
  
  class PushVar : public IInstruction
  {
  public:
    virtual bool canExec() const override
    { 
      if (name_.empty())
      {
        setError("PushVar[CAN] No variable was specified.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      if (stack->getLocal(name_) != nullptr)
      {
        setError("PushVar[EXE] Variable of same name already on the stack. '" + name_ + "'.");
        return false;
      }

      Var* var = new Var();
      stack->push(name_, var);

      return true;
    }

    void setName(const std::string& name) { name_ = name; }

  private:
    std::string name_;
  };

  class PopVar : public IInstruction
  {
  public:
    virtual bool canExec() const override
    { 
      if (name_.empty())
      {
        setError("PopVar[CAN] No variable was specified.");
        return false;
      }

      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Var* var = stack->pop(name_);
      if (var != nullptr)
      {
        delete var;
        return true;
      }
      
      setError("PopVar[EXE] Could not find name to pop off of the stack. '" + name_ + "'.");
      return false;
    }

    void setName(const std::string& name) { name_ = name; }


  private:
    std::string name_;
  };

  class SetVar : public IInstruction
  {
  public:
    virtual bool canExec() const override
    {
      if (name_.empty() || data_.empty() || type_ == kVarUnknown)
      {
        setError("SetVar[CAN] An input was not set properly - Name: '" + name_ + "' Data: '" + std::string(data_.begin(), data_.end()) + "' Type: '" + typeToStr(type_) + "'.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Var* var = stack->get(name_);

      if (var == nullptr)
      {
        setError("SetVar[EXE] Variable does not exist in the current scope: '" + name_ + "'.");
        return false;
      }

      var->make(data_, type_);

      return true;
    }

    void setName(const std::string& name)       { name_ = name; }
    void setData(const std::string& s) { data_.resize(s.size() + 1u); memcpy(data_.data(), s.data(), s.size() + 1u); type_ = kVarString; }
    void setData(const float& r)       { data_.resize(sizeof(float)); memcpy(data_.data(), &r, sizeof(float));       type_ = kVarReal;   }
    void setData(const bool& b)        { data_.resize(sizeof(bool));  memcpy(data_.data(), &b, sizeof(bool));        type_ = kVarBool;   }
    void setData(const char* s)        { setData(std::string(s)); }

  private:
    std::string       name_;
    std::vector<char> data_;
    VarType           type_ = kVarUnknown;
  };

  class AssignVar : public IInstruction
  {
  public:
    virtual bool canExec() const override
    {
      if (name_lhs_.empty() || name_rhs_.empty())
      {
        setError("AssignVar[CAN] An input was not set properly - Name LHS: '" + name_lhs_ + "' Name RHS: '" + name_rhs_ + "'.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Var* lhs = stack->get(name_lhs_);
      Var* rhs = stack->get(name_rhs_);

      if (lhs == nullptr || rhs == nullptr)
      {
        setError("AssignVar[EXE] Either the LHS variable or the RHS variable was invalid - Name LHS: '" + name_lhs_ + "' Valid LHS: '" + std::to_string(lhs != nullptr) + "' Name RHS: '" + name_rhs_ + "'Valid RHS: '" + std::to_string(rhs != nullptr) + "' .");
        return false;
      }

      switch (rhs->getType())
      {
      case kVarReal:
        lhs->make(rhs->asReal());
        return true;
      case kVarString:
        lhs->make(rhs->asString());
        return true;
      case kVarBool:
        lhs->make(rhs->asBool());
        return true;
      case kVarUnknown:
        lhs->make(0.0f);
        return true;
      }

      setError("AssignVar[EXE] Somehow it got to the end of the function. Something unknown happened - Name LHS: '" + name_lhs_ + "' Valid LHS: '" + std::to_string(lhs != nullptr) + "' Name RHS: '" + name_rhs_ + "'Valid RHS: '" + std::to_string(rhs != nullptr) + "' .");
      return false;
    }

    void setNameLhs(const std::string& name) { name_lhs_ = name; }
    void setNameRhs(const std::string& name) { name_rhs_ = name; }

  private:
    std::string name_lhs_;
    std::string name_rhs_;
  };

  enum Operators
  {
    kOpAdd,
    kOpSubtract,
    kOpDivide,
    kOpMultiply,

    kOpEqual,
    kOpNotEqual,
    kOpGreaterThan,
    kOpGreaterThanOrEqual,
    kOpLessThan,
    kOpLessThanOrEqual,
    
    kOpUnknown
  };

  class MathOpVar : public IInstruction
  {
  public:
    virtual bool canExec() const override
    {
      if (name_lhs_.empty() || name_rhs_.empty() || name_result_.empty() || op_ == kOpUnknown)
      {
        setError("MathOpVar[CAN] An input was not set properly - Name LHS: '" + name_lhs_ + "' Name RHS: '" + name_rhs_ + "' Name Result: '" + name_result_ + "' operator: '" + std::to_string(op_) + "'.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Var* lhs = stack->get(name_lhs_);
      Var* rhs = stack->get(name_rhs_);
      Var* result = stack->get(name_result_);

      if (lhs == nullptr || rhs == nullptr || result == nullptr)
      {
        setError("MathOpVar[EXE] Either LHS, RHS or Result was not on the stack - Name LHS: '" + name_lhs_ + "' Valid LHS: '" + std::to_string(lhs != nullptr) + "' Name RHS: '" + name_rhs_ + "' Valid RHS: '" + std::to_string(rhs != nullptr) + "' Name Result: '" + name_result_ + "' Valid Result: '" + std::to_string(result != nullptr) + "' operator: '" + std::to_string(op_) + "'.");
        return false;
      }

      // Handle the unknown types.
      if (lhs->getType() == kVarUnknown)
      {
        lhs->convert(kVarReal);
      }
      if (rhs->getType() == kVarUnknown)
      {
        rhs->convert(kVarReal);
      }

      switch (op_)
      {
      case kOpAdd:
        add(lhs, rhs, result);
        break;
      case kOpSubtract:
        subtract(lhs, rhs, result);
        break;
      case kOpMultiply:
        multiply(lhs, rhs, result);
        break;
      case kOpDivide:
        divide(lhs, rhs, result);
        break;
      }

      return true;
    }

  private:
    void add(Var* lhs, Var* rhs, Var* result) const
    {
      switch (lhs->getType())
      {
      case kVarReal:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make(lhs->asReal() + rhs->asReal());
          break;
        case kVarString:
          result->make(lhs->asReal() + stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make(lhs->asReal() + (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      case kVarString:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make(stringToReal(lhs->asString()) + rhs->asReal());
          break;
        case kVarString:
          result->make(lhs->asString() + rhs->asString());
          break;
        case kVarBool:
          result->make(stringToReal(lhs->asString()) + (rhs->asBool() ? true : false) > 0.0f ? true : false);
          break;
        }
        break;
      case kVarBool:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make((lhs->asBool() ? 1.0f : 0.0f) + rhs->asReal());
          break;
        case kVarString:
          result->make((lhs->asBool() ? 1.0f : 0.0f) + stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make((lhs->asBool() ? 1.0f : 0.0f) + (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      }
    }
    void subtract(Var* lhs, Var* rhs, Var* result) const
    {
      switch (lhs->getType())
      {
      case kVarReal:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make(lhs->asReal() - rhs->asReal());
          break;
        case kVarString:
          result->make(lhs->asReal() - stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make(lhs->asReal() - (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      case kVarString:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make(stringToReal(lhs->asString()) - rhs->asReal());
          break;
        case kVarString:
          result->make(stringToReal(lhs->asString()) - stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make(stringToReal(lhs->asString()) - (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      case kVarBool:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make((lhs->asBool() ? 1.0f : 0.0f) - rhs->asReal());
          break;
        case kVarString:
          result->make((lhs->asBool() ? 1.0f : 0.0f) - stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make((lhs->asBool() ? 1.0f : 0.0f) - (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      }
    }
    void multiply(Var* lhs, Var* rhs, Var* result) const
    {
      switch (lhs->getType())
      {
      case kVarReal:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make(lhs->asReal() * rhs->asReal());
          break;
        case kVarString:
          result->make(lhs->asReal() * stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make(lhs->asReal() * (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      case kVarString:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make(stringToReal(lhs->asString()) * rhs->asReal());
          break;
        case kVarString:
          result->make(stringToReal(lhs->asString()) * stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make(stringToReal(lhs->asString()) * (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      case kVarBool:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make((lhs->asBool() ? 1.0f : 0.0f) * rhs->asReal());
          break;
        case kVarString:
          result->make((lhs->asBool() ? 1.0f : 0.0f) * stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make((lhs->asBool() ? 1.0f : 0.0f) * (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      }
    }
    void divide(Var* lhs, Var* rhs, Var* result) const
    {
      switch (lhs->getType())
      {
      case kVarReal:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make(lhs->asReal() / rhs->asReal());
          break;
        case kVarString:
          result->make(lhs->asReal() / stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make(lhs->asReal() / (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      case kVarString:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make(stringToReal(lhs->asString()) / rhs->asReal());
          break;
        case kVarString:
          result->make(stringToReal(lhs->asString()) / stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make(stringToReal(lhs->asString()) / (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      case kVarBool:
        switch (rhs->getType())
        {
        case kVarReal:
          result->make((lhs->asBool() ? 1.0f : 0.0f) / rhs->asReal());
          break;
        case kVarString:
          result->make((lhs->asBool() ? 1.0f : 0.0f) / stringToReal(rhs->asString()));
          break;
        case kVarBool:
          result->make((lhs->asBool() ? 1.0f : 0.0f) / (rhs->asBool() ? 1.0f : 0.0f));
          break;
        }
        break;
      }
    }

  public:
    void setNameLhs(const std::string& name) { name_lhs_ = name; }
    void setNameRhs(const std::string& name) { name_rhs_ = name; }
    void setNameResult(const std::string& name) { name_result_ = name; }
    void setOp(const Operators& op) { op_ = op; }

  private:
    std::string name_lhs_;
    std::string name_rhs_;
    std::string name_result_;
    Operators   op_ = kOpUnknown;
  };

  class CompareOpVar : public IInstruction
  {
    virtual bool canExec() const override
    {
      if (name_lhs_.empty() || name_rhs_.empty() || op_ == kOpUnknown)
      {
        setError("CompareOpVar[CAN] An input was not set properly - Name LHS: '" + name_lhs_ + "' Name RHS: '" + name_rhs_ + "' operator: '" + std::to_string(op_) + "'.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Var* var_lhs = stack->get(name_lhs_);
      Var* var_rhs = stack->get(name_rhs_);

      if (var_lhs == nullptr || var_rhs == nullptr)
      {
        setError("CompareOpVar[EXE] Either LHS or RHS was not on the stack - Name LHS: '" + name_lhs_ + "' Valid LHS: '" + std::to_string(var_lhs != nullptr) + "' Name RHS: '" + name_rhs_ + "' Valid RHS: '" + std::to_string(var_rhs != nullptr) + "' operator: '" + std::to_string(op_) + "'.");
        return false;
      }

      if (var_lhs->getType() == kVarString && var_rhs->getType() == kVarString)
      {
        return var_lhs->asString() == var_rhs->asString();
      }

      float lhs = 0.0f;
      switch (var_lhs->getType())
      {
      case kVarReal:
        lhs = var_lhs->asReal();
        break;
      case kVarString:
        lhs = stringToReal(var_lhs->asString());
        break;
      case kVarBool:
        lhs = var_lhs->asBool() ? 1.0f : 0.0f;
        break;
      }

      float rhs = 0.0f;
      switch (var_rhs->getType())
      {
      case kVarReal:
        rhs = var_rhs->asReal();
        break;
      case kVarString:
        rhs = stringToReal(var_rhs->asString());
        break;
      case kVarBool:
        rhs = var_rhs->asBool() ? 1.0f : 0.0f;
        break;
      }

      switch (op_)
      {
      case kOpEqual:
        return lhs == rhs;
      case kOpNotEqual:
        return lhs != rhs;
      case kOpGreaterThan:
        return lhs > rhs;
      case kOpGreaterThanOrEqual:
        return lhs >= rhs;
      case kOpLessThan:
        return lhs < rhs;
      case kOpLessThanOrEqual:
        return lhs <= rhs;
      }

      return false;
    }

  public:
    void setNameLhs(const std::string& name) { name_lhs_ = name; }
    void setNameRhs(const std::string& name) { name_rhs_ = name; }
    void setOp(const Operators& op) { op_ = op; }
    Operators getOp() const { return op_; }

  private:
    std::string name_lhs_;
    std::string name_rhs_;
    Operators op_ = kOpUnknown;
  };

  class PrintVar : public IInstruction
  {
  public:
    virtual bool canExec() const override
    {
      if (name_.empty())
      {
        setError("PrintVar[CAN] The name was not set - Name: '" + name_ + "'.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Var* var = stack->get(name_);
      if (var == nullptr)
      {
        setError("PrintVar[EXE] The variable was not on the stack - Name: '" + name_ + "'.");
        return false;
      }

#ifdef _DEBUG
      std::cout << name_ << ": ";
#endif

      switch (var->getType())
      {
      case kVarUnknown:
        std::cout << "mysterious" << std::endl;
        return true;
      case kVarReal:
        std::cout << std::to_string(var->asReal()) << std::endl;
        return true;
      case kVarString:
        std::cout << var->asString() << std::endl;
        return true;
      case kVarBool:
        std::cout << (var->asBool() ? "true" : "false") << std::endl;
        return true;
      }
      return false;
    }

    void setName(const std::string& name) { name_ = name; }


  private:
    std::string name_;
  };

  class ListenVar : public IInstruction
  {
  public:
    virtual bool canExec() const override
    {
      if (name_.empty())
      {
        setError("ListenVar[CAN] The name was not set - Name: '" + name_ + "'.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Var* var = stack->get(name_);
      
      if (var == nullptr)
      {
        setError("ListenVar[EXE] The variable was not on the stack - Name: '" + name_ + "'.");
        return false;
      }

      std::string line;
      std::getline(std::cin, line);

      if (isStrReal(line))
      {
        var->make(std::stof(line));
      }
      else if (equals(line, k_true_aliases))
      {
        var->make(true);
      }
      else if (equals(line, k_false_aliases))
      {
        var->make(false);
      }
      else if (equals(line, k_null_aliases))
      {
        var->make(0.0f);
      }
      else if (line.size() >= 1u && line.at(0u) == '"')
      {
        var->make(line.substr(1u, line.size() - 2u));
      }
      else
      {
        var->make(stringToReal(line));
      }

      return true;
    }

    void setName(const std::string& name) { name_ = name; }


  private:
    std::string name_;
  };

  class ConvertVar : public IInstruction
  {
  public:
    virtual bool canExec() const override
    {
      if (name_.empty() && type_ != kVarUnknown)
      {
        setError("ConvertVar[CAN] Either the name was not set, or the type was unknown - Name: '" + name_ + "' Type: '" + std::to_string(type_) + "'.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Var* var = stack->get(name_);
      if (var != nullptr)
      {
        if (var->convert(type_))
        {
          return true;
        }
        setError("ConvertVar[EXE] Failed to convert the variable - Name: '" + name_ + "'.");
        return false;
      }
      setError("ConvertVar[EXE] The variable was not on the stack - Name: '" + name_ + "'.");
      return false;
    }

    void setName(const std::string& name) { name_ = name; }
    void setType(const VarType& type)     { type_ = type; }


  private:
    std::string name_;
    VarType type_ = kVarUnknown;
  };

  class ScopeInstruction : public IInstruction
  {
    virtual bool canExec() const override
    {
      if (scope_.empty())
      {
        setError("ScopeInstruction[CAN] The scope was not set - Scope: '" + scope_ + "'.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Scope scope = ScopeManager::getScopeManager()->getScope(scope_);
      scope.execute();
      return true;
    }

  public:
    void setScope(const std::string& scope) { scope_ = scope; }

  private:
    std::string scope_;
  };

  class ReturnScopeInstruction : public IInstruction
  {
    virtual bool canExec() const override
    {
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Scope* scope = Scope::getCurrentScope();

      std::vector<Scope*> scopes_to_kill;
      bool found = false;

      while (scope != nullptr && found == false)
      {
        if (contains(scope->getName(), "__func_"))
        {
          found = true;
        }

        scopes_to_kill.push_back(scope);
        scope = scope->getParent();
      }

      if (false == found)
      {
        setError("ReturnScopeInstruction[EXE] Could not find a function to return from.");
        return false;
      }

      for (Scope* scope : scopes_to_kill)
      {
        scope->stopExecuting();
      }

      return true;
    }
  };

  class ContinueScopeInstruction : public IInstruction
  {
    virtual bool canExec() const override
    {
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Scope* scope = Scope::getCurrentScope();

      std::vector<Scope*> scopes_to_kill;
      bool found = false;

      while (scope != nullptr && found == false)
      {
        if (contains(scope->getName(), "__while_") || contains(scope->getName(), "__until_"))
        {
          found = true;
        }

        scopes_to_kill.push_back(scope);
        scope = scope->getParent();
      }

      if (false == found)
      {
        setError("ContinueScopeInstruction[EXE] Could not find a loop to contine.");
        return false;
      }

      scopes_to_kill.back()->skipThisRound();
      scopes_to_kill.pop_back();
      for (Scope* scope : scopes_to_kill)
      {
        scope->stopExecuting();
      }

      return true;
    }
  };

  class BreakScopeInstruction : public IInstruction
  {
    virtual bool canExec() const override
    {
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Scope* scope = Scope::getCurrentScope();

      std::vector<Scope*> scopes_to_kill;
      bool found = false;

      while (scope != nullptr && found == false)
      {
        if (contains(scope->getName(), "__while_") || contains(scope->getName(), "__until_"))
        {
          found = true;
        }

        scopes_to_kill.push_back(scope);
        scope = scope->getParent();
      }

      if (false == found)
      {
        setError("BreakScopeInstruction[EXE] Could not find a loop to break out of.");
        return false;
      }

      for (Scope* scope : scopes_to_kill)
      {
        scope->stopExecuting();
      }

      return true;
    }
  };

  class ImportInstruction : public IInstruction
  {
    virtual bool canExec() const override
    {
      if (name_.empty())
      {
        setError("ImportInstruction[CAN] The name was not set - Name: '" + name_ + "'.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Var* global = ScopeManager::getScopeManager()->getUserData(name_);
      if (global == nullptr)
      {
        setError("ImportInstruction[EXE] Could not find the user data - Name: '" + name_ + "'.");
        return false;
      }

      Var* local = stack->get(name_);
      if (local == nullptr)
      {
        setError("ImportInstruction[EXE] Could not find the local variable - Name: '" + name_ + "'.");
        return false;
      }

      local->make(global);
      return true;
    }

  public:
    void setName(const std::string& name) { name_ = name; }

  private:
    std::string name_;
  };

  class ExportInstruction : public IInstruction
  {
    virtual bool canExec() const override
    {
      if (name_.empty())
      {
        setError("ExportInstruction[CAN] The name was not set - Name: '" + name_ + "'.");
        return false;
      }
      return true;
    }

    virtual bool exec(Stack* stack) const override
    {
      Var* local = stack->get(name_);
      if (local == nullptr)
      {
        setError("ExportInstruction[EXE] Could not find the local variable - Name: '" + name_ + "'.");
        return false;
      }

      Var* global = new Var;
      global->make(local);
      ScopeManager::getScopeManager()->setUserData(name_, global);
      return true;
    }

  public:
    void setName(const std::string& name) { name_ = name; }

  private:
    std::string name_;
  };
}
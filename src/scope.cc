#include "scope.h"
#include <vector>
#include "instruction.h"
#include "variable.h"

namespace lambda
{
  std::vector<Scope*> Scope::g_scope_stack_;

  void Scope::initialize()
  {
    skip_this_round_ = false;
    keep_executing_ = true;
    if (false == g_scope_stack_.empty())
    {
      setParent(g_scope_stack_.back());
      stack_.setParent(getParent()->getStack());
    }
    g_scope_stack_.push_back(this);
  }
  void Scope::deinitialize()
  {
    std::vector<Var*> vars = stack_.clear();
    for (Var* var : vars)
    {
      delete var;
    }

    g_scope_stack_.pop_back();
  }
  void Scope::execute()
  {
    keep_executing_ = true;
    while (keep_executing_)
    {
      initialize();
    
      for (uint32_t i = 0u; i < blockers_.size() && keep_executing_ && !skip_this_round_; ++i)
      {
        IInstruction* instruction = blockers_.at(i);
        if (false == (instruction->canExec() && instruction->exec(&stack_)))
        {
          keep_executing_ = false;
        }
      }

      for (uint32_t i = 0u; i < instructions_.size() && keep_executing_ && !skip_this_round_; ++i)
      {
        IInstruction* instruction = instructions_.at(i);

        if (false == (instruction->canExec() && instruction->exec(&stack_)))
        {
          std::cout << "[ERR] " << instruction->errorMessage() << std::endl;
        }
      }

      if (!repeat_until_fail_)
      {
        keep_executing_ = false;
      }

      deinitialize();
    }
  }
  void Scope::free()
  {
    parent_ = nullptr;
    for (IInstruction* instruction : instructions_)
    {
      delete instruction;
    }
    for (IInstruction* blocker : blockers_)
    {
      delete blocker;
    }
  }
  void Scope::setName(const std::string& name)
  {
    name_ = name;
  }
  std::string Scope::getName() const
  {
    return name_;
  }
  void Scope::setParent(Scope* parent)
  {
    parent_ = parent;
  }
  Scope* Scope::getParent()
  {
    return parent_;
  }
  Stack* Scope::getStack()
  {
    return &stack_;
  }
  void Scope::stopExecuting()
  {
    keep_executing_ = false;
  }
  void Scope::makeRepeat()
  {
    repeat_until_fail_ = true;
  }
  void Scope::skipThisRound()
  {
    skip_this_round_ = true;
  }
  void Scope::addInstruction(IInstruction* instruction)
  {
    instructions_.push_back(instruction);
  }
  void Scope::addBlocker(IInstruction* blocker)
  {
    blockers_.push_back(blocker);
  }
  void Scope::addBlockers(const std::vector<IInstruction*>& blockers)
  {
    blockers_.insert(blockers_.end(), blockers.begin(), blockers.end());
  }
  Scope* Scope::getCurrentScope()
  {
    return g_scope_stack_.back();
  }

  ScopeManager* ScopeManager::g_scope_manager_ = nullptr;

  ScopeManager* ScopeManager::getScopeManager()
  {
    return g_scope_manager_;
  }
  Scope ScopeManager::getScope(const std::string& name)
  {
    for (Scope& scope : scopes_)
    {
      if (scope.getName() == name)
      {
        return scope;
      }
    }

    return Scope();
  }
  Scope& ScopeManager::getScopeRef(const std::string& name)
  {
    for (Scope& scope : scopes_)
    {
      if (scope.getName() == name)
      {
        return scope;
      }
    }
    std::runtime_error("ERR: Could not find scope:" + name);
    return scopes_.at(0u);
  }
  void ScopeManager::addScope(Scope scope)
  {
    scopes_.push_back(scope);
  }
  void ScopeManager::exec()
  {
    g_scope_manager_ = this;
    Scope::g_scope_stack_.push_back(&user_data_);
    getScope("main").execute();
  }
  void ScopeManager::free()
  {
    for (Scope& scope : scopes_)
    {
      scope.free();
    }
    scopes_.clear();

    // Delete user data.
    for (Var* var : user_data_.getStack()->clear())
    {
      delete var;
    }
  }
  void ScopeManager::setUserData(const std::string& name, Var* var)
  {
    if (user_data_.getStack()->get(name) != nullptr)
    {
      delete user_data_.getStack()->pop(name);
    }
    user_data_.getStack()->push(name, var);
  }
  Var* ScopeManager::getUserData(const std::string& name)
  {
    return user_data_.getStack()->get(name);
  }
}
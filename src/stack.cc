#include "stack.h"
#include "variable.h"

namespace lambda
{
  void Stack::push(const std::string& name, Var* var)
  {
    if (variables_.find(name) == variables_.end())
    {
      variables_.insert(std::make_pair(name, var));
    }
  }
  Var* Stack::pop(const std::string& name)
  {
    auto it = variables_.find(name);
    if (it == variables_.end())
    {
      return nullptr;
    }

    Var* var = it->second;
    variables_.erase(it);
    return var;
  }
  Var * Stack::getLocal(const std::string& name)
  {
    auto it = variables_.find(name);
    if (it != variables_.end())
    {
      return it->second;
    }
    return nullptr;
  }
  std::vector<Var*> Stack::clear()
  {
    std::vector<Var*> vars;

    for (const auto& it : variables_)
    {
      vars.push_back(it.second);
    }
    variables_.clear();

    return vars;
  }
  Var* Stack::get(const std::string& name)
  {
    Var* var = getLocal(name);
    if (var != nullptr)
    {
      return var;
    }
    if (parent_ != nullptr)
    {
      return parent_->get(name);
    }
    else
    {
      return nullptr;
    }
  }
  void Stack::setParent(const Stack* parent)
  {
    parent_ = (Stack*)parent;
  }
}
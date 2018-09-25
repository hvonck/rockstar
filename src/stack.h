#pragma once
#include <map>
#include <vector>

namespace lambda
{
  class Var;
  class Stack
  {
  public:
    // Local stack only functions.
    void push(const std::string& name, Var* var);
    Var* pop(const std::string& name);
    Var* getLocal(const std::string& name);
    std::vector<Var*> clear();
    
    // Local and parent stack functions.
    Var* get(const std::string& name);
    void setParent(const Stack* parent);

  private:
    std::map<std::string, Var*> variables_;
    Stack* parent_ = nullptr;
  };
}
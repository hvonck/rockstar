#pragma once
#include "stack.h"

namespace lambda
{
  class IInstruction;

  class Scope
  {
  public:
    void initialize();
    void deinitialize();
    void execute();
    void free();

    void setName(const std::string& name);
    std::string getName() const;
    void setParent(Scope* parent);
    Scope* getParent();
    Stack* getStack();
    void stopExecuting();
    void makeRepeat();
    void skipThisRound();

    void addInstruction(IInstruction* instruction);
    void addBlocker(IInstruction* blocker);
    void addBlockers(const std::vector<IInstruction*>& blockers);

  private:
    Stack stack_;
    std::string name_;
    Scope* parent_ = nullptr;
    bool keep_executing_;
    bool skip_this_round_ = false;
    bool repeat_until_fail_ = false;
    std::vector<IInstruction*> instructions_;
    std::vector<IInstruction*> blockers_;

  public:
    static Scope* getCurrentScope();

  protected:
    static std::vector<Scope*> g_scope_stack_;

    friend class ScopeManager;
  };

  class ScopeManager
  {
  public:
    static ScopeManager* getScopeManager();

    Scope getScope(const std::string& name);
    Scope& getScopeRef(const std::string& name);
    void addScope(Scope scope);
    void exec();
    void free();
    void setUserData(const std::string& name, Var* var);
    Var* getUserData(const std::string& name);

  private:
    Scope user_data_;
    std::vector<Scope> scopes_;

  private:
    static ScopeManager* g_scope_manager_;
  };
}
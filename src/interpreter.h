#pragma once
#include "instruction.h"
#include "scope.h"

namespace lambda
{
  struct OpData
  {
    enum Type
    {
      kNumber,
      kAdd,
      kSubtract,
      kMultiply,
      kDivide
    };

    OpData(const std::string& val, const Type& type) :
      val(val),
      type(type)
    {}
    std::string val;
    Type type = Type::kNumber;
  };
  struct OpWeight
  {
    OpWeight(const size_t& idx, const size_t& count, const OpData::Type& type) :
      idx(idx)
    {
      // Multiply and divide are way more important than add and subtract.
      switch (type)
      {
      case OpData::kAdd:      weight = (count - idx) + 0u;   break;
      case OpData::kSubtract: weight = (count - idx) + 0u;   break;
      case OpData::kMultiply: weight = (count - idx) + count; break;
      case OpData::kDivide:   weight = (count - idx) + count; break;
      }
    }
    bool operator<(const OpWeight& other) const
    {
      return (weight < other.weight);
    }
    size_t weight;
    size_t idx;
  };
  struct TempScope
  {
    TempScope(std::string name) : name(name) {}
    TempScope(std::string name, std::string parent) : name(name), parent(parent) {}
    std::string name = "";
    std::string parent = "";
    uint32_t temp_var_count = 0u;
    uint32_t scope_count = 0u;
    std::string last_var = "";
    std::vector<std::string> variables;
  };

  class Interpreter
  {
  public:
    ScopeManager interpret(const char* file);

  private:
    bool interpretLine(const char* line);
    bool commSays(const std::vector<std::string>& words, Scope& scope);
    bool commIs(const std::vector<std::string>& words, Scope& scope);
    bool commPut(const std::vector<std::string>& words, Scope& scope);
    bool commTakes(const std::vector<std::string>& words, Scope& scope);
    bool commTaking(const std::vector<std::string>& words, Scope& scope);
    bool commGive(const std::vector<std::string>& words, Scope& scope);
    bool commSay(const std::vector<std::string>& words, Scope& scope);
    bool commMaths(const std::vector<std::string>& words, Scope& scope);
    bool commIf(const std::vector<std::string>& words, Scope& scope);
    bool commElseIf(const std::vector<std::string>& words, Scope& scope);
    bool commElse(const std::vector<std::string>& words, Scope& scope);
    bool commWhile(const std::vector<std::string>& words, Scope& scope);
    bool commUntil(const std::vector<std::string>& words, Scope& scope);
    bool commBuild(const std::vector<std::string>& words, Scope& scope);
    bool commKnock(const std::vector<std::string>& words, Scope& scope);
    bool commListen(const std::vector<std::string>& words, Scope& scope);
    bool commContinue(const std::vector<std::string>& words, Scope& scope);
    bool commBreak(const std::vector<std::string>& words, Scope& scope);
    bool commImport(const std::vector<std::string>& words, Scope& scope);
    bool commExport(const std::vector<std::string>& words, Scope& scope);

    std::vector<IInstruction*> commCompare(const std::vector<std::string>& words);
    bool commSet(const std::string& name, const std::string& value, Scope& scope, bool create = true);
    bool commElseWithBlockers(Scope& scope, std::string name, const std::vector<IInstruction*>& blockers);
    std::string pushTempVar(Scope& scope);
    void popTempVar(Scope& scope);
    void getOperators(const std::vector<std::string>& line, std::vector<OpData>& operators, Scope& scope);
    void weightOperators(const std::vector<OpData>& operators, std::vector<OpWeight>& weights);
    bool hasOpLeft(const std::vector<OpData>& operators, const size_t& idx);
    bool hasOpRight(const std::vector<OpData>& operators, const size_t& idx);
    void createQueue(const std::vector<OpData>& operators, const std::vector<OpWeight>& weights, Scope& scope);
    void pushVar(const std::string& name, Scope& scope);
    void popVar(const std::string& name, Scope& scope);
    bool hasVar(const std::string& name, Scope& scope);
    std::string getVarName(const std::string& name, Scope& scope);
    TempScope& getTempScope(const std::string& name);
    void requireVar(const std::string& name, Scope& scope);
    void addScope(const Scope& new_scope, const Scope& current_scope);
    void useVar(const std::string& name, Scope& scope);
    bool validVar(const std::string& name);

  private:
    std::vector<TempScope> scope_stack_;
    ScopeManager scope_manager_;
    std::string last_temp_var_;
  };
}
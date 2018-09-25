#include "interpreter.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <algorithm>

/*
Comments: (this is a comment)

*/

namespace lambda
{
  ScopeManager Interpreter::interpret(const char* file)
  {
    scope_manager_ = ScopeManager();

    Scope main_scope;
    main_scope.setName("main");
    scope_manager_.addScope(main_scope);
    scope_stack_.push_back(TempScope{ "main" });

    for (const std::string& line : split(file, '\n'))
    {
      interpretLine(line.c_str());
    }
    
    return scope_manager_;
  }
  bool Interpreter::interpretLine(const char* line)
  {
    std::string l = line;
    if (l.empty())
    {
      if (scope_stack_.size() > 1u)
      {
        scope_stack_.pop_back();
      }
      return false;
    }
    l = cleanup(l);

    // Check if this is in fact correct.
    l = replaceAll(l, "'s ", " is ");

    Scope& scope = scope_manager_.getScopeRef(scope_stack_.back().name);
    std::vector<std::string> words = split(l, ' ');
    std::vector<std::string> lower_words = toLower(words);

    if (words.empty())
    {
      if (scope_stack_.size() > 1u)
      {
        scope_stack_.pop_back();
      }
      return true;
    }
    if (equals(lower_words.at(0u), "put"))
    {
      return commPut(words, scope);
    }
    if (equals(lower_words.at(0u), k_say_aliases))
    {
      return commSay(words, scope);
    }
    if (equals(lower_words.at(0u), "if"))
    {
      return commIf(words, scope);
    }
    if (equals(lower_words.at(0u), "else"))
    {
      if (words.size() > 2u && equals(lower_words.at(1u), "if"))
      {
        return commElseIf(words, scope);
      }
      else
      {
        return commElse(words, scope);
      }
    }
    if (equals(lower_words.at(0u), "while"))
    {
      return commWhile(words, scope);
    }
    if (equals(lower_words.at(0u), "until"))
    {
      return commUntil(words, scope);
    }
    if (equals(lower_words.at(0u), "build"))
    {
      return commBuild(words, scope);
    }
    if (equals(lower_words.at(0u), "knock"))
    {
      return commKnock(words, scope);
    }
    if (words.size() >= 2u && equals(lower_words.at(0u), "listen") && equals(lower_words.at(1u), "to"))
    {
      return commListen(words, scope);
    }
    if (equals(lower_words.at(0u), "give"))
    {
      return commGive(words, scope);
    }
    if (equals(lower_words.at(0u), "give"))
    {
      return commGive(words, scope);
    }
    if (equals(lower_words.at(0u), "import"))
    {
      return commImport(words, scope);
    }
    if (equals(lower_words.at(0u), "export"))
    {
      return commExport(words, scope);
    }
    if (toLower(l).find("break it down") != std::string::npos)
    {
      return commBreak(words, scope);
    }
    if (toLower(l).find("take it to the top") != std::string::npos)
    {
      return commContinue(words, scope);
    }

    // These all require looping
    // The words can never be the first or the last.
    for (uint32_t i = 1u; i < words.size() - 1u; ++i)
    {
      std::string word = lower_words.at(i);
      if (equals(word, "says"))
      {
        return commSays(words, scope);
        return true;
      }
      if (equals(word, k_is_aliases))
      {
        return commIs(words, scope);
      }
      if (equals(word, "taking"))
      {
        return commTaking(words, scope);
      }
      if (equals(word, "takes"))
      {
        return commTakes(words, scope);
      }
      if (equals(word, k_add_aliases) ||
        equals(word, k_subtract_aliases) ||
        equals(word, k_multiply_aliases) ||
        equals(word, k_divide_aliases))
      {
        return commMaths(words, scope);
      }
    }

    return false;
  }
  bool Interpreter::commSays(const std::vector<std::string>& words, Scope& scope)
  {
    uint32_t idx = UINT32_MAX;
    for (uint32_t i = 0u; i < words.size() && idx == UINT32_MAX; ++i)
    {
      if (equals(toLower(words.at(i)), "says"))
      {
        idx = i;
      }
    }

    if (idx == UINT32_MAX) throw std::runtime_error("Could not find word that it requires");

    std::string name  = vecToStr(std::vector<std::string>(words.begin(), words.begin() + idx));
    std::string value = vecToStr(std::vector<std::string>(words.begin() + idx + 1u, words.end()));
    name = getVarName(name, scope);
    useVar(name, scope);

    std::string temp_name = pushTempVar(scope);

    // TODO (Hilze): Can propbably skip some of this.
    pushVar(temp_name, scope);
    SetVar* set_temp = new SetVar;
    set_temp->setName(temp_name);
    set_temp->setData(value);
    scope.addInstruction(set_temp);

    requireVar(name, scope);
    AssignVar* assign_name = new AssignVar;
    assign_name->setNameLhs(name);
    assign_name->setNameRhs(temp_name);
    scope.addInstruction(assign_name);

    popVar(temp_name, scope);
    popTempVar(scope);

    return true;
  }
  bool Interpreter::commIs(const std::vector<std::string>& words, Scope& scope)
  {
    uint32_t idx = UINT32_MAX;
    for (uint32_t i = 0u; i < words.size() && idx == UINT32_MAX; ++i)
    {
      if (equals(toLower(words.at(i)), k_is_aliases))
      {
        idx = i;
      }
    }

    if (idx == UINT32_MAX) throw std::runtime_error("Could not find word that it requires");

    std::string name  = vecToStr(std::vector<std::string>(words.begin(), words.begin() + idx));
    std::string value = vecToStr(std::vector<std::string>(words.begin() + idx + 1u, words.end()));

    return commSet(name, value, scope);
  }
  bool Interpreter::commPut(const std::vector<std::string>& words, Scope& scope)
  {
    uint32_t idx = UINT32_MAX;
    for (uint32_t i = 0u; i < words.size() && idx == UINT32_MAX; ++i)
    {
      if (equals(toLower(words.at(i)), "into"))
      {
        idx = i;
      }
    }

    if (idx == UINT32_MAX) throw std::runtime_error("Could not find word that it requires");

    std::string value = vecToStr(std::vector<std::string>(words.begin() + 1u, words.begin() + idx));
    std::string name  = vecToStr(std::vector<std::string>(words.begin() + idx + 1u, words.end()));

    return commSet(name, value, scope);
  }
  bool Interpreter::commTakes(const std::vector<std::string>& words, Scope& scope)
  {
    uint32_t idx = UINT32_MAX;
    for (uint32_t i = 0u; i < words.size() && idx == UINT32_MAX; ++i)
    {
      if (equals(toLower(words.at(i)), "takes"))
      {
        idx = i;
      }
    }

    if (idx == UINT32_MAX) throw std::runtime_error("Could not find word that it requires");

    std::string name = vecToStr(std::vector<std::string>(words.begin(), words.begin() + idx));
    std::vector<std::string> arguments = split(vecToStr(std::vector<std::string>(words.begin() + idx + 1u, words.end())), "and");
    
    // Create new scope.
    std::string scope_name = "__func_" + name + "__";
    Scope new_scope;
    new_scope.setName(scope_name);
    addScope(new_scope, scope);
    Scope& ns = scope_manager_.getScopeRef(new_scope.getName());

    // Add all arguments and copy them over ones.
    for (size_t i = 0u; i < arguments.size(); ++i)
    {
      std::string arg_name = cleanup(arguments.at(i));
      if (arg_name.at(0u) == ' ') arg_name.erase(arg_name.begin());
      
      pushVar(arg_name, ns);
      AssignVar* assign_arg = new AssignVar;
      assign_arg->setNameLhs(arg_name);
      assign_arg->setNameRhs("__func_arg" + std::to_string(i) + "__");

      ns.addInstruction(assign_arg);
    }

    return true;
  }
  bool Interpreter::commTaking(const std::vector<std::string>& words, Scope& scope)
  {
    uint32_t idx = UINT32_MAX;
    for (uint32_t i = 0u; i < words.size() && idx == UINT32_MAX; ++i)
    {
      if (equals(toLower(words.at(i)), "taking"))
      {
        idx = i;
      }
    }

    if (idx == UINT32_MAX) throw std::runtime_error("Could not find word that it requires");

    std::string name = vecToStr(std::vector<std::string>(words.begin(), words.begin() + idx));
    std::vector<std::string> arguments = split(vecToStr(std::vector<std::string>(words.begin() + idx + 1u, words.end())), ',');

    // Add all arguments and copy them over ones.
    for (size_t i = 0u; i < arguments.size(); ++i)
    {
      std::string arg_val = arguments.at(i);
      if (arg_val.at(0u) == ' ') arg_val.erase(arg_val.begin());
      std::string arg_name = "__func_arg" + std::to_string(i) + "__";

      commSet(arg_name, arg_val, scope, true);
    }

    // Add the return variable.
    pushVar("__func_ret__", scope);
    
    // Call the scope.
    ScopeInstruction* call_scope = new ScopeInstruction;
    call_scope->setScope("__func_" + name + "__");
    scope.addInstruction(call_scope);

    // Copy the return variable over to a temporary variable.
    std::string temp_name = pushTempVar(scope);
    pushVar(temp_name, scope);
    AssignVar* assign_temp = new AssignVar;
    assign_temp->setNameLhs(temp_name);
    assign_temp->setNameRhs("__func_ret__");
    scope.addInstruction(assign_temp);

    for (size_t i = 0u; i < arguments.size(); ++i)
    {
      std::string arg_name = "__func_arg" + std::to_string(i) + "__";
      popVar(arg_name, scope);
    }
    
    // Pop the return variable.
    popVar("__func_ret__", scope);

    return true;
  }
  bool Interpreter::commGive(const std::vector<std::string>& words, Scope& scope)
  {
    std::string name = vecToStr(std::vector<std::string>(words.begin() + 2u, words.end()));

    if (interpretLine(name.c_str())) name = last_temp_var_;

    AssignVar* assign_ret = new AssignVar;
    assign_ret->setNameLhs("__func_ret__");
    assign_ret->setNameRhs(name);
    scope.addInstruction(assign_ret);
    ReturnScopeInstruction* end_scope = new ReturnScopeInstruction;
    scope.addInstruction(end_scope);

    return true;
  }
  bool Interpreter::commSay(const std::vector<std::string>& words, Scope& scope)
  {
    std::string name = cleanup(vecToStr(std::vector<std::string>(words.begin() + 1u, words.end())));

    if (false == hasVar(name, scope))
    {
      // Create temp var.
      std::string temp_name = pushTempVar(scope);
      pushVar(temp_name, scope);
      
      // Assign temp var.
      commSet(temp_name, name, scope, false);
      
      // Print temp var.
      PrintVar* print_var = new PrintVar;
      print_var->setName(temp_name);
      scope.addInstruction(print_var);
      
      // Pop temp var.
      popVar(temp_name, scope);
      popTempVar(scope);
    }
    else if (name.at(0u) != '"' && interpretLine(name.c_str()))
    {
      // Print the var.
      PrintVar* print_var = new PrintVar;
      print_var->setName(last_temp_var_);
      scope.addInstruction(print_var);
    }
    else
    {
      name = getVarName(name, scope);
      useVar(name, scope);
      // Print the var.
      PrintVar* print_var = new PrintVar;
      print_var->setName(name);
      scope.addInstruction(print_var);
    }

    return true;
  }
  void Interpreter::getOperators(const std::vector<std::string>& line, std::vector<OpData>& operators, Scope& scope)
  {
    auto name_check = [=](const std::string& name, Scope& scope) {
      if (!hasVar(name, scope))
      {
        std::string nm = pushTempVar(scope);
        commSet(nm, name, scope, true);
        return nm;
      }
      std::string nm = getVarName(name, scope);
      useVar(nm, scope);
      return nm;
    };

    std::string name;
    for (size_t i = 0u; i < line.size(); ++i)
    {
      const std::string& word = line.at(i);
      if (equals(toLower(word), k_add_aliases))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name_check(name, scope), OpData::kNumber });
          name.clear();
        }
        operators.push_back(OpData{ word, OpData::kAdd });
      }
      else if (equals(toLower(word), k_subtract_aliases))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name_check(name, scope), OpData::kNumber });
          name.clear();
        }
        operators.push_back(OpData{ word, OpData::kSubtract });
      }
      else if (equals(toLower(word), k_multiply_aliases))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name_check(name, scope), OpData::kNumber });
          name.clear();
        }
        operators.push_back(OpData{ word, OpData::kMultiply });
      }
      else if (equals(toLower(word), k_divide_aliases))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name_check(name, scope), OpData::kNumber });
          name.clear();
        }
        operators.push_back(OpData{ word, OpData::kDivide });
      }
      else
      {
        name = name.empty() ? word : (name + " " + word);
      }
    }
    if (false == name.empty())
    {
      operators.push_back(OpData{ name_check(name, scope), OpData::kNumber });
      name.clear();
    }
  }
  void Interpreter::weightOperators(const std::vector<OpData>& operators, std::vector<OpWeight>& weights)
  {
    for (size_t i = 0u; i < operators.size(); ++i)
    {
      if (operators.at(i).type != OpData::kNumber)
      {
        weights.push_back(OpWeight(i, operators.size(), operators.at(i).type));
      }
    }

    std::sort(weights.rbegin(), weights.rend());
  }
  bool Interpreter::hasOpLeft(const std::vector<OpData>& operators, const size_t& idx)
  {
    for (size_t i = 0u; i < idx; ++i)
    {
      if (operators.at(i).type != OpData::kNumber)
      {
        return true;
      }
    }
    return false;
  }
  bool Interpreter::hasOpRight(const std::vector<OpData>& operators, const size_t& idx)
  {
    for (size_t i = idx + 1u; i < operators.size(); ++i)
    {
      if (operators.at(i).type != OpData::kNumber)
      {
        return true;
      }
    }
    return false;
  }
  void Interpreter::createQueue(const std::vector<OpData>& operators, const std::vector<OpWeight>& weights, Scope& scope)
  {
    // Operator to temporary values.
    std::map<size_t, std::string> types;

    for (const OpWeight& weight : weights)
    {
      const OpData& op = operators.at(weight.idx);
      std::string lhs;
      std::string rhs;

      // Find out what the lhs should be.
      if (hasOpLeft(operators, weight.idx) && types.find(weight.idx - 2u) != types.end())
      {
        lhs = types.at(weight.idx - 2u);
      }
      else
      {
        lhs = operators.at(weight.idx - 1u).val;
      }

      // Find out what the rhs should be.
      if (hasOpRight(operators, weight.idx) && types.find(weight.idx + 2u) != types.end())
      {
        rhs = types.at(weight.idx + 2u);
      }
      else
      {
        rhs = operators.at(weight.idx + 1u).val;
      }

      // Create the new temporary value.
      std::string tmp = pushTempVar(scope);
      types[weight.idx] = tmp;
      pushVar(tmp, scope);
      
      // Create the action.
      MathOpVar* math_op = new MathOpVar;
      math_op->setNameLhs(lhs);
      math_op->setNameRhs(rhs);
      math_op->setNameResult(tmp);

      switch (op.type)
      {
      case OpData::kAdd:      math_op->setOp(kOpAdd);      break;
      case OpData::kSubtract: math_op->setOp(kOpSubtract); break;
      case OpData::kMultiply: math_op->setOp(kOpMultiply); break;
      case OpData::kDivide:   math_op->setOp(kOpDivide);   break;
      }
      scope.addInstruction(math_op);
    }
  }
  void Interpreter::pushVar(const std::string& name, Scope& scope)
  {
    
    if (!validVar(name))
    {
      throw std::runtime_error("Not a valid variable!");
    }

    TempScope& ts = getTempScope(scope.getName());
    ts.variables.push_back(name);

    PushVar* push_var = new PushVar;
    push_var->setName(name);
    scope.addInstruction(push_var);
  }
  void Interpreter::popVar(const std::string& name, Scope& scope)
  {
    TempScope& ts = getTempScope(scope.getName());
    
    for (uint32_t i = 0u; i < ts.variables.size(); ++i)
    {
      if (equals(ts.variables.at(i), name))
      {
        ts.variables.erase(ts.variables.begin() + i);

        PopVar* pop_var = new PopVar;
        pop_var->setName(name);
        scope.addInstruction(pop_var);

        return;
      }
    }

    throw std::runtime_error("Could not pop variable from stack. Variable might be in a different scope. " + name);
  }
  bool Interpreter::hasVar(const std::string& name, Scope& scope)
  {
    std::string n = name;
    if (equals(name, k_it_aliases))
    {
      n = getTempScope(scope.getName()).last_var;
      if (n.empty())
      {
        throw std::runtime_error("ERR Tried to use it even though no variable was used yet");
      }
    }

    TempScope& ts = getTempScope(scope.getName());

    for (uint32_t i = 0u; i < ts.variables.size(); ++i)
    {
      if (equals(ts.variables.at(i), n))
      {
        return true;
      }
    }

    if (false == ts.parent.empty())
    {
      return hasVar(n, scope_manager_.getScopeRef(ts.parent));
    }

    return false;
  }
  std::string Interpreter::getVarName(const std::string& name, Scope& scope)
  {
    if (equals(name, k_it_aliases))
    {
      std::string n = getTempScope(scope.getName()).last_var;
      if (n.empty())
      {
        throw std::runtime_error("ERR Tried to use it even though no variable was used yet");
      }
      return n;
    }
    return name;
  }
  TempScope& Interpreter::getTempScope(const std::string& name)
  {
    for (int32_t i = (int32_t)scope_stack_.size() - 1; i >= 0; --i)
    {
      if (scope_stack_.at(i).name == name)
      {
        return scope_stack_.at(i);
      }
    }

    throw std::runtime_error("Could not find temp scope of name " + name);
    return scope_stack_.back();
  }
  void Interpreter::requireVar(const std::string& name, Scope& scope)
  {
    if (!hasVar(name, scope))
    {
      pushVar(name, scope);
    }
  }
  void Interpreter::addScope(const Scope& new_scope, const Scope& current_scope)
  {
    std::string current_scope_name = current_scope.getName();
    std::string new_scope_name = new_scope.getName();
    scope_manager_.addScope(new_scope);
    scope_stack_.push_back(TempScope{ new_scope_name, current_scope_name });
  }
  void Interpreter::useVar(const std::string& name, Scope& scope)
  {
    if (name.find("__") == std::string::npos && false == equals(name, k_it_aliases))
    {
      getTempScope(scope.getName()).last_var = name;
    }
    else if (equals(name, k_it_aliases))
    {
      int i = 0;
    }
  }
  bool Interpreter::validVar(const std::string& name)
  {
    bool throw_error = true;
    if (startsWith(name, "__"))
    {
      return true;
    }
    else if (isupper(name.at(0u)))
    {
      return true;
    }
    else if (startsWith(name, k_common_aliases_) && isLower(name))
    {
      return true;
    }
    return false;
  }
  bool Interpreter::commMaths(const std::vector<std::string>& words, Scope& scope)
  {
    std::vector<OpData> operators;
    std::vector<OpWeight> weights;
    getOperators(words, operators, scope);
    weightOperators(operators, weights);
    createQueue(operators, weights, scope);

    return true;
  }
  bool Interpreter::commIf(const std::vector<std::string>& words, Scope& scope)
  {
    // Create new scope.
    std::string scope_name = "__if_" + std::to_string(++getTempScope(scope.getName()).scope_count) + "__";
    Scope new_scope;
    new_scope.setName(scope_name);

    // Add a blocker.
    new_scope.addBlockers(commCompare(std::vector<std::string>(words.begin() + 1u, words.end())));

    // Add a set succeeded to false instruction to the old stack.
    std::string if_succeeded = "__if_" + std::to_string(getTempScope(scope.getName()).scope_count) + "_succeeded__";
    pushVar(if_succeeded, scope);

    SetVar* reset_if_succeeded = new SetVar;
    reset_if_succeeded->setName(if_succeeded);
    reset_if_succeeded->setData(false);
    scope.addInstruction(reset_if_succeeded);

    // Add a start scope instruction.
    ScopeInstruction* scope_start = new ScopeInstruction;
    scope_start->setScope(scope_name);
    scope.addInstruction(scope_start);

    // Add a set succeeded to true instruction to the new stack.
    SetVar* set_if_succeeded = new SetVar;
    set_if_succeeded->setName(if_succeeded);
    set_if_succeeded->setData(true);
    new_scope.addInstruction(set_if_succeeded);

    // Add new scope to the manager and make it the main scope.
    addScope(new_scope, scope);

    return true;
  }
  bool Interpreter::commElseIf(const std::vector<std::string>& words, Scope& scope)
  {
    return commElseWithBlockers(scope, "else_if", commCompare(std::vector<std::string>(words.begin() + 2u, words.end())));
  }
  bool Interpreter::commElse(const std::vector<std::string>& words, Scope& scope)
  {
    return commElseWithBlockers(scope, "else", {});
  }
  bool Interpreter::commWhile(const std::vector<std::string>& words, Scope& scope)
  {
    // Create new scope.
    std::string scope_name = "__while_" + std::to_string(++getTempScope(scope.getName()).scope_count) + "__";
    Scope new_scope;
    new_scope.setName(scope_name);
    new_scope.makeRepeat();

    // Add a blocker.
    new_scope.addBlockers(commCompare(std::vector<std::string>(words.begin() + 1u, words.end())));

    // Add a start scope instruction.
    ScopeInstruction* scope_start = new ScopeInstruction;
    scope_start->setScope(scope_name);
    scope.addInstruction(scope_start);

    // Add new scope to the manager and make it the main scope.
    addScope(new_scope, scope);

    return true;
  }
  bool Interpreter::commUntil(const std::vector<std::string>& words, Scope& scope)
  {
    // Create new scope.
    std::string scope_name = "__until_" + std::to_string(++getTempScope(scope.getName()).scope_count) + "__";
    Scope new_scope;
    new_scope.setName(scope_name);
    new_scope.makeRepeat();

    // Get a blocker.
    std::vector<IInstruction*> blockers = commCompare(std::vector<std::string>(words.begin() + 1u, words.end()));
    
    // Invert blocker.
    for (IInstruction* it : blockers)
    {
      CompareOpVar* blocker = (CompareOpVar*)it;

      Operators op = blocker->getOp();
      if (op == kOpEqual) op = kOpNotEqual;
      else if (op == kOpNotEqual) op = kOpEqual;
      else if (op == kOpGreaterThan) op = kOpLessThanOrEqual;
      else if (op == kOpLessThanOrEqual) op = kOpGreaterThan;
      else if (op == kOpLessThan) op = kOpGreaterThanOrEqual;
      else if (op == kOpGreaterThanOrEqual) op = kOpLessThan;
      blocker->setOp(op);
    }
    // Add a blocker.
    new_scope.addBlockers(blockers);

    // Add a start scope instruction.
    ScopeInstruction* scope_start = new ScopeInstruction;
    scope_start->setScope(scope_name);
    scope.addInstruction(scope_start);

    // Add new scope to the manager and make it the main scope.
    addScope(new_scope, scope);

    return true;
  }
  bool Interpreter::commBuild(const std::vector<std::string>& words, Scope& scope)
  {
    std::string name = vecToStr(std::vector<std::string>(words.begin() + 1u, words.end() - 1u));
    std::string temp_var = pushTempVar(scope);

    pushVar(temp_var, scope);
    name = getVarName(name, scope);
    useVar(name, scope);
    requireVar(name, scope);

    SetVar* set_var = new SetVar;
    set_var->setName(temp_var);
    set_var->setData(1.0f);
    scope.addInstruction(set_var);

    MathOpVar* math_op = new MathOpVar;
    math_op->setNameLhs(name);
    math_op->setNameResult(name);
    math_op->setNameRhs(temp_var);
    math_op->setOp(kOpAdd);
    scope.addInstruction(math_op);

    popVar(temp_var, scope);
    popTempVar(scope);

    return true;
  }
  bool Interpreter::commKnock(const std::vector<std::string>& words, Scope& scope)
  {
    std::string name = vecToStr(std::vector<std::string>(words.begin() + 1u, words.end() - 1u));
    std::string temp_var = pushTempVar(scope);

    pushVar(temp_var, scope);
    name = getVarName(name, scope);
    useVar(name, scope);
    requireVar(name, scope);

    SetVar* set_var = new SetVar;
    set_var->setName(temp_var);
    set_var->setData(1.0f);
    scope.addInstruction(set_var);

    MathOpVar* math_op = new MathOpVar;
    math_op->setNameLhs(name);
    math_op->setNameResult(name);
    math_op->setNameRhs(temp_var);
    math_op->setOp(kOpSubtract);
    scope.addInstruction(math_op);

    popVar(temp_var, scope);
    popTempVar(scope);

    return true;
  }
  bool Interpreter::commListen(const std::vector<std::string>& words, Scope& scope)
  {
    std::string name = vecToStr(std::vector<std::string>(words.begin() + 2u, words.end()));
    name = getVarName(name, scope);
    useVar(name, scope);

    requireVar(name, scope);
    ListenVar* listen_var = new ListenVar;
    listen_var->setName(name);
    scope.addInstruction(listen_var);

    return true;
  }
  bool Interpreter::commContinue(const std::vector<std::string>& words, Scope& scope)
  {
    scope.addInstruction(new ContinueScopeInstruction);

    return true;
  }
  bool Interpreter::commBreak(const std::vector<std::string>& words, Scope& scope)
  {
    scope.addInstruction(new BreakScopeInstruction);

    return true;
  }
  bool Interpreter::commImport(const std::vector<std::string>& words, Scope& scope)
  {
    std::string name = vecToStr(std::vector<std::string>(words.begin() + 1u, words.end()));
    name = getVarName(name, scope);
    useVar(name, scope);
    requireVar(name, scope);
    ImportInstruction* imp = new ImportInstruction;
    imp->setName(name);
    scope.addInstruction(imp);

    return true;
  }
  bool Interpreter::commExport(const std::vector<std::string>& words, Scope& scope)
  {
    std::string name = vecToStr(std::vector<std::string>(words.begin() + 1u, words.end()));
    name = getVarName(name, scope);
    useVar(name, scope);
    ExportInstruction* exp = new ExportInstruction;
    exp->setName(name);
    scope.addInstruction(exp);

    return true;
  }
  std::vector<IInstruction*> Interpreter::commCompare(const std::vector<std::string>& w)
  {
    std::vector<IInstruction*> blockers;
    Scope& scope = scope_manager_.getScope(scope_stack_.back().name);

    for (const std::string& s : split(vecToStr(w), " and "))
    {
      std::vector<std::string> words = split(cleanup(s), ' ');

      bool inverse = false;
      uint32_t idx = UINT32_MAX;
      for (uint32_t i = 0u; i < words.size() && idx == UINT32_MAX; ++i)
      {
        if (equals(toLower(words.at(i)), "is"))
        {
          idx = i;
        }
        else if (equals(toLower(words.at(i)), "ain't"))
        {
          idx = i;
          inverse = !inverse;
        }
      }

      if (idx == UINT32_MAX) throw std::runtime_error("Could not find word that it requires");

      std::string lhs = vecToStr(std::vector<std::string>(words.begin(), words.begin() + idx));
      if (hasVar(lhs, scope))
      {
        lhs = getVarName(lhs, scope);
        useVar(lhs, scope);
      }
      else if (interpretLine(lhs.c_str()))
      {
        lhs = last_temp_var_;
      }

      std::vector<std::string> rhs = std::vector<std::string>(words.begin() + idx + 1u, words.end());
      if (equals(toLower(rhs.at(0u)), "not"))
      {
        inverse = !inverse;
        rhs.erase(rhs.begin());
      }

      CompareOpVar* compare_op = new CompareOpVar;
      compare_op->setNameLhs(lhs);

      if (equals(toLower(rhs.at(0u)), k_greater_than_aliases))
      {
        rhs.erase(rhs.begin(), rhs.begin() + 2u);
        compare_op->setOp(inverse ? kOpLessThanOrEqual : kOpGreaterThan);
      }
      else if (equals(toLower(rhs.at(0u)), k_less_than_aliases))
      {
        rhs.erase(rhs.begin(), rhs.begin() + 2u);
        compare_op->setOp(inverse ? kOpGreaterThanOrEqual : kOpLessThan);
      }
      else if (equals(toLower(rhs.at(0u)), "as"))
      {
        if (equals(toLower(rhs.at(1u)), k_greater_than_or_equal_to_aliases))
        {
          rhs.erase(rhs.begin(), rhs.begin() + 3u);
          compare_op->setOp(inverse ? kOpLessThan : kOpGreaterThanOrEqual);
        }
        else if (equals(toLower(rhs.at(1u)), k_less_than_or_equal_to_aliases))
        {
          rhs.erase(rhs.begin(), rhs.begin() + 3u);
          compare_op->setOp(inverse ? kOpGreaterThan : kOpLessThanOrEqual);
        }
      }
      else
      {
        compare_op->setOp(inverse ? kOpNotEqual : kOpEqual);
      }

      std::string name_rhs = vecToStr(rhs);
      if (hasVar(name_rhs, scope))
      {
        name_rhs = getVarName(name_rhs, scope);
        useVar(lhs, scope);
      }
      else
      {
        if (interpretLine(name_rhs.c_str()))
        {
          name_rhs = last_temp_var_;
        }
        else
        {
          std::string temp_var = pushTempVar(scope_manager_.getScopeRef(scope_stack_.back().name));
          commSet(temp_var, name_rhs, scope_manager_.getScopeRef(scope_stack_.back().name));
          name_rhs = temp_var;
        }
      }

      compare_op->setNameRhs(name_rhs);

      blockers.push_back(compare_op);
    }

    return blockers;
  }
  bool Interpreter::commSet(const std::string& name, const std::string& value, Scope& scope, bool create)
  {
    std::string nm = getVarName(name, scope);
    
    if (!validVar(nm))
    {
      return false;
    }

    useVar(nm, scope);
    
    if (create)
    {
      requireVar(nm, scope);
    }

    if (hasVar(value, scope))
    {
      AssignVar* assign_var = new AssignVar;
      assign_var->setNameLhs(name);
      assign_var->setNameRhs(getVarName(value, scope));
      scope.addInstruction(assign_var);
    }
    else if (value.at(0u) != '"' && interpretLine(value.c_str()))
    {
      AssignVar* assign_var = new AssignVar;
      assign_var->setNameLhs(nm);
      assign_var->setNameRhs(last_temp_var_);
      scope.addInstruction(assign_var);
    }
    else 
    {
      SetVar* set_var = new SetVar;
      set_var->setName(nm);

      if (isStrReal(value))
      {
        set_var->setData(std::stof(value));
      }
      else if (equals(toLower(value), k_true_aliases))
      {
        set_var->setData(true);
      }
      else if (equals(toLower(value), k_false_aliases))
      {
        set_var->setData(false);
      }
      else if (equals(toLower(value), k_null_aliases))
      {
        set_var->setData(0.0f);
      }
      else if (value.at(0u) == '"')
      {
        set_var->setData(value.substr(1u, value.size() - 2u));
      }
      else
      {
        set_var->setData(stringToReal(value));
      }
      scope.addInstruction(set_var);
    }

    return true;
  }
  bool Interpreter::commElseWithBlockers(Scope& scope, std::string name, const std::vector<IInstruction*>& blockers)
  {
    // Create new scope.
    std::string scope_name = "__" + name + "_" + std::to_string(getTempScope(scope.getName()).scope_count) + "__";
    Scope new_scope;
    new_scope.setName(scope_name);

    // Add a false var.
    std::string false_var = pushTempVar(scope);
    pushVar(false_var, scope);
    SetVar* set_false_var = new SetVar;
    set_false_var->setName(false_var);
    set_false_var->setData(false);
    scope.addInstruction(set_false_var);

    // Add a blocker.
    CompareOpVar* blocker = new CompareOpVar;
    std::string if_succeeded = "__if_" + std::to_string(getTempScope(scope.getName()).scope_count) + "_succeeded__";
    blocker->setNameLhs(false_var);
    blocker->setNameRhs(if_succeeded);
    blocker->setOp(kOpEqual);
    new_scope.addBlocker(blocker);

    new_scope.addBlockers(blockers);

    // Add a start scope instruction.
    ScopeInstruction* scope_start = new ScopeInstruction;
    scope_start->setScope(scope_name);
    scope.addInstruction(scope_start);

    // Pop the false var.
    popVar(false_var, scope);
    popTempVar(scope);

    // Add a set succeeded to true instruction to the new stack.
    SetVar* set_if_succeeded = new SetVar;
    set_if_succeeded->setName(if_succeeded);
    set_if_succeeded->setData(true);
    new_scope.addInstruction(set_if_succeeded);

    // Add new scope to the manager and make it the main scope.
    addScope(new_scope, scope);

    return true;
  }
  std::string Interpreter::pushTempVar(Scope& scope)
  {
    last_temp_var_ = "__temp_" + std::to_string(++getTempScope(scope.getName()).temp_var_count) + "__";
    return last_temp_var_;
  }
  void Interpreter::popTempVar(Scope& scope)
  {
    last_temp_var_ = "__temp_" + std::to_string(--getTempScope(scope.getName()).temp_var_count) + "__";
  }
}
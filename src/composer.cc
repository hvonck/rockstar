#include "composer.h"
#include <sstream>
#include <iostream>
#include <algorithm>

namespace lambda
{
  Composer::Composer()
  {
    pronouns_ = {
      "it",
      "he",
      "she",
      "him",
      "her",
      "them",
      "they"
    };
    null_ = {
      "null",
      "nothing",
      "nowhere",
      "nobody"
    };
    true_ = {
      "true",
      "right",
      "yes",
      "ok"
    };
    false_ = {
      "false",
      "wrong",
      "no",
      "lies"
    };
    plus_ = {
      "plus",
      "with"
    };
    minus_ = {
      "minus",
      "without"
    };
    multiply_ = {
      "times",
      "of"
    };
    divide_ = {
      "over"
    };
    increment_ = {
      "build"
    };
    decrement_ = {
      "knock"
    };
    put_ = {
      "put"
    };
    into_ = {
      "into"
    };
    is_ = {
      "is"
    };
    cout_ = {
      "say",
      "shout",
      "whisper",
      "scream"
    };
    cin_ = {
      "listen"
    };

    for (const String& null : null_)
    {
      TypeContainer* null_container = rockConstruct<TypeContainer>();
      null_container->construct(null, rockConstruct<NullType>());
      variables_.push_back(null_container);
    }
  }
  
  Vector<String> Composer::lineToWords(const String& line)
  {
    Vector<String> words = rockSplit(line, ' ');

    // Remove all comments.
    for (size_t i = 0u; i < words.size(); ++i)
    {
      if (false == words.at(i).empty() && words.at(i).at(0u) == '(')
      {
        words.erase(words.begin() + i, words.end());
        break;
      }
    }

    return std::move(words);
  }
  Vector<String> toLower(const Vector<String>& words)
  {
    Vector<String> lower;
    for (String word : words)
    {
      for (char& ch : word)
      {
        ch = tolower(ch);
      }
      lower.push_back(word);
    }
    return lower;
  }
  void Composer::lineToActions(Vector<ActionContainer>& actions, const String& line)
  {
    Vector<String> words = lineToWords(line);
    Vector<String> lower = toLower(words);

    if (words.empty())
    {
      // TODO (Hilze): Add end of function, end of loop and end of if here.
      return;
    }
    if (rockEqual(lower.at(0u), increment_))
    {
      increment(actions, words);
    }
    else if (rockEqual(lower.at(0u), decrement_))
    {
      decrement(actions, words);
    }
    else if (rockEqual(lower.at(0u), put_))
    {
      put(actions, words);
    }
    else if (rockEqual(lower.at(0u), cout_))
    {
      cout(actions, words);
    }
    else if (rockEqual(lower.at(0u), cin_) && rockEqual(lower.at(1u), "to"))
    {
      cin(actions, words);
    }
    else if (rockEqual(lower, is_))
    {
      is(actions, words);
    }
    else if (rockEqual(lower, plus_) || rockEqual(lower, minus_) || rockEqual(lower, multiply_) || rockEqual(lower, divide_))
    {
      arithmetic(actions, words);
    }
  }
  Song Composer::compose(const String& file)
  {
    Vector<String> lines = rockSplit(file, '\n');
    size_t total_size = 0u;

    for (const String& line : lines)
    {
      total_size += line.size();
      lineToActions(actions_, line);
    }

    return Song(generateSongName(total_size), generateArtistName(total_size), std::move(variables_), std::move(actions_));
  }

  void Composer::increment(Vector<ActionContainer>& actions, const Vector<String>& line)
  {
    ActionContainer action;
    action.construct(rockConstruct<IncrementAction>());
    action.asIAction()->setType(getType(rockToString(line, 1u, line.size() - 1u)));
    actions.push_back(action);
  }
  void Composer::decrement(Vector<ActionContainer>& actions, const Vector<String>& line)
  {
    ActionContainer action;
    action.construct(rockConstruct<DecrementAction>());
    action.asIAction()->setType(getType(rockToString(line, 1u, line.size() - 1u)));
    actions.push_back(action);
  }
  void Composer::assign(Vector<ActionContainer>& actions, const String& name, const String& value)
  {
    if (value.at(0u) == '\"')
    {
      ActionContainer action;
      action.construct(rockConstruct<AssignStringAction>());
      action.asIAction()->setType(getType(name));

      TypeContainer* temporary = newTemporary();
      temporary->construct(temporary->getName(), rockConstruct<StringType>());
      temporary->asString()->set(value.substr(1u, value.size() - 2u));
      action.asAssignString()->setAssign(temporary);

      actions.push_back(action);
    }
    else if (rockEqual(value, true_))
    {
      ActionContainer action;
      action.construct(rockConstruct<AssignBoolAction>());
      action.asAssignBool()->setType(getType(name));

      TypeContainer* temporary = newTemporary();
      temporary->construct(temporary->getName(), rockConstruct<BoolType>());
      temporary->asBool()->set(true);
      action.asAssignString()->setAssign(temporary);

      actions.push_back(action);
    }
    else if (rockEqual(value, false_))
    {
      ActionContainer action;
      action.construct(rockConstruct<AssignBoolAction>());
      action.asAssignBool()->setType(getType(name));

      TypeContainer* temporary = newTemporary();
      temporary->construct(temporary->getName(), rockConstruct<BoolType>());
      temporary->asBool()->set(false);
      action.asAssignString()->setAssign(temporary);

      actions.push_back(action);
    }
    else
    {
      TypeContainer* result = nullptr;
      float f = 0.0f;
      if (rockIsFloat(value, &f))
      {
        result = newTemporary();
        result->construct(result->getName(), rockConstruct<NumberType>());
        result->asNumber()->set(f);
      }
      else if (hasType(value))
      {
        result = getType(value);
      }
      else
      {
        result = subExecute(actions, value);
        if (result == nullptr)
        {
          result = newTemporary();
          result->construct(result->getName(), rockConstruct<NumberType>());

          float number = rockStringToNumber(value);

          result->asNumber()->set(number);
        }
      }

      ActionContainer action;
      action.construct(rockConstruct<AssignNumberAction>());
      action.asAssignNumber()->setType(getType(name));
      action.asAssignNumber()->setAssign(result);
      actions.push_back(action);
    }
  }
  void Composer::put(Vector<ActionContainer>& actions, const Vector<String>& line)
  {
    size_t idx = 0u;
    for (idx = 0u; idx < line.size(); ++idx)
    {
      if (rockEqual(line.at(idx), into_))
      {
        break;
      }
    }

    String value = rockToString(line, 1u, idx);
    String name  = rockToString(line, idx + 1u);
    assign(actions, name, value);
  }
  void Composer::is(Vector<ActionContainer>& actions, const Vector<String>& line)
  {
    size_t idx = 0u;
    for (idx = 0u; idx < line.size(); ++idx)
    {
      if (rockEqual(line.at(idx), is_))
      {
        break;
      }
    }

    String name  = rockToString(line, 0u, idx);
    String value = rockToString(line, idx + 1u);
    assign(actions, name, value);
  }

  void Composer::cout(Vector<ActionContainer>& actions, const Vector<String>& line)
  {
    String name = rockToString(line, 1u);

    ActionContainer action;
    action.construct(rockConstruct<COutAction>());
    action.asIAction()->setType(getType(name));
    actions.push_back(action);
  }

  void Composer::cin(Vector<ActionContainer>& actions, const Vector<String>& line)
  {
    String name = rockToString(line, 2u);

    ActionContainer action;
    action.construct(rockConstruct<CInAction>());
    action.asIAction()->setType(getType(name));
    actions.push_back(action);
  }
  
  void Composer::getOperators(const Vector<String>& line, Vector<OpData>& operators)
  {
    String name;
    for (size_t i = 0u; i < line.size(); ++i)
    {
      const String& word = line.at(i);
      if (rockEqual(word, plus_))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name, OpData::kNumber });
          name.clear();
        }
        operators.push_back(OpData{ word, OpData::kAdd });
        continue;
      }
      else if (rockEqual(word, minus_))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name, OpData::kNumber });
          name.clear();
        }
        operators.push_back(OpData{ word, OpData::kSubtract });
        continue;
      }
      if (rockEqual(word, multiply_))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name, OpData::kNumber });
          name.clear();
        }
        operators.push_back(OpData{ word, OpData::kMultiply });
        continue;
      }
      if (rockEqual(word, divide_))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name, OpData::kNumber });
          name.clear();
        }
        operators.push_back(OpData{ word, OpData::kDivide });
        continue;
      }
      name = name.empty() ? word : (name + " " + word);
    }
    if (false == name.empty())
    {
      operators.push_back(OpData{ name, OpData::kNumber });
      name.clear();
    }
  }
  void Composer::weightOperators(const Vector<OpData>& operators, Vector<OpWeight>& weights)
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
  bool Composer::hasOpLeft(const Vector<OpData>& operators, const size_t& idx)
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
  bool Composer::hasOpRight(const Vector<OpData>& operators, const size_t& idx)
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
  void Composer::createQueue(const Vector<OpData>& operators, const Vector<OpWeight>& weights, Vector<ActionContainer>& actions)
  {
    // Operator to temporary values.
    Map<size_t, TypeContainer*> types;

    for (const OpWeight& weight : weights)
    {
      const OpData& op = operators.at(weight.idx);
      TypeContainer* lhs = nullptr;
      TypeContainer* rhs = nullptr;
      
      // Find out what the lhs should be.
      if (hasOpLeft(operators, weight.idx) && types.find(weight.idx - 2u) != types.end())
      {
        lhs = types.at(weight.idx - 2u);
      }
      else
      {
        lhs = getType(operators.at(weight.idx - 1u).val);
      }

      // Find out what the rhs should be.
      if (hasOpRight(operators, weight.idx) && types.find(weight.idx + 2u) != types.end())
      {
        rhs = types.at(weight.idx + 2u);
      }
      else
      {
        rhs = getType(operators.at(weight.idx + 1u).val);
      }

      // Create the new temporary value.
      TypeContainer* tmp = newTemporary();
      types.insert(std::make_pair(weight.idx, tmp));

      // Create the action.
      ActionContainer action;
      switch (op.type)
      {
      case OpData::kAdd:      action.construct(rockConstruct<PlusAction>());     break;
      case OpData::kSubtract: action.construct(rockConstruct<MinusAction>());    break;
      case OpData::kMultiply: action.construct(rockConstruct<MultiplyAction>()); break;
      case OpData::kDivide:   action.construct(rockConstruct<DivideAction>());   break;
      }
      action.asArithmetic()->setLeft(lhs);
      action.asArithmetic()->setRight(rhs);
      action.asArithmetic()->setType(tmp);
      actions.push_back(action);
    }
  }
  void Composer::arithmetic(Vector<ActionContainer>& actions, const Vector<String>& line)
  {
    Vector<OpData> operators;
    Vector<OpWeight> weights;
    getOperators(line, operators);
    weightOperators(operators, weights);
    createQueue(operators, weights, actions);
  }
  bool Composer::hasType(const String& name) const
  {
    for (TypeContainer* type : variables_)
    {
      if (rockEqual(type->getName(), name))
      {
        return true;
      }
    }

    return false;
  }

  TypeContainer* Composer::getType(const String& name)
  {
    for (TypeContainer* type : variables_)
    {
      if (rockEqual(type->getName(), name))
      {
        return type;
      }
    }

    TypeContainer* var = rockConstruct<TypeContainer>();
    var->setName(name);
    variables_.push_back(var);

    return getType(name);
  }
  TypeContainer* Composer::newTemporary()
  {
    String str = "temporary_" + std::to_string(temporary_variables_++);
    return getType(str);
  }
  TypeContainer* Composer::lastTemporary()
  {
    return getTemporary(temporary_variables_ - 1u);
  }
  TypeContainer* Composer::getTemporary(size_t idx)
  {
    String str = "temporary_" + std::to_string(idx);
    return getType(str);
  }

  TypeContainer* Composer::subExecute(Vector<ActionContainer>& actions, const String& value)
  {
    size_t action_count = actions.size();
    lineToActions(actions, value);

    if (actions.size() != action_count)
    {
      return lastTemporary();
    }
    else
    {
      return nullptr;
    }
  }

}
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
      "is",
      "was",
      "were"
    };
    says_ = {
      "says",
      "said"
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
    if_ = {
      "if"
    };
    else_ = {
      "else"
    };
    while_ = {
      "while"
    };
    until_ = {
      "until"
    };
    greater_ = {
      "higher",
      "greater",
      "bigger",
      "stronger"
    };
    less_ = {
      "lower",
      "less",
      "smaller",
      "weaker"
    };
    greater_equal_ = {
      "high",
      "great",
      "big",
      "strong"
    };
    less_equal_ = {
      "low",
      "little",
      "small",
      "weak"
    };
    takes_ = {
      "takes"
    };
    taking_ = {
      "taking"
    };
    give_ = {
      "give"
    };
  }
  
  Vector<String> Composer::lineToWords(const String& line)
  {
    Vector<char> special_characters = {
      '.',
      ',',
      '?',
      '!',
    };

    Vector<String> words = rockSplit(line, ' ');

    // Remove all comments.
    for (size_t i = 0u; i < words.size();)
    {
      if (false == words.at(i).empty() && words.at(i).at(0u) == '(')
      {
        words.erase(words.begin() + i, words.end());
        break;
      }
      // Remove all trailing spaces
      for (size_t j = 0u; j < words.at(i).size(); ++j)
      {
        if (words.at(i).at(j) == ' ')
        {
          words.at(i).erase(words.at(i).begin() + j, words.at(i).end());
          break;
        }
      }
      // Remove empty words
      if (words.at(i).empty())
      {
        words.erase(words.begin() + i);
        continue;
      }
      else
      {
        ++i;
      }
    }

    if (words.size() > 0u && words.back().size() > 0u && rockEqual(words.back().back(), special_characters))
    {
      words.back().pop_back();
    }

    return words;
  }
  Vector<String> toLower(const Vector<String>& words)
  {
    Vector<String> lower = words;
    for (String& word : lower)
    {
      word = rockToLower(word);
    }
    return lower;
  }
  void Composer::lineToActions(const String& line)
  {
    Vector<String> words = lineToWords(line);
    Vector<String> lower = toLower(words);

    if (words.empty())
    {
      if (rockEqual(current_couplette_->name, "while") || rockEqual(current_couplette_->name, "until"))
      {
        ActionContainer* action = newAction(current_couplette_);
        action->construct(rockConstruct<CoupletteAction>());
        action->asCouplette()->setCouplette(current_couplette_);
      }

      if (current_couplette_->parent != nullptr)
      {
        current_couplette_ = current_couplette_->parent;
      }
      if (rockEqual(current_couplette_->name, "root"))
      {
        current_couplette_ = current_couplette_->children.at(0u);
      }
    }
    else if (rockEqual(lower.at(0u), if_))
    {
      ifStatement(words);
    }
    else if (rockEqual(lower.at(0u), else_))
    {
      elseStatement(words);
    }
    else if (rockEqual(lower.at(0u), while_))
    {
      whileStatement(words);
    }
    else if (rockEqual(lower.at(0u), until_))
    {
      untilStatement(words);
    }
    else if (rockEqual(lower.at(0u), increment_))
    {
      increment(current_couplette_, words);
    }
    else if (rockEqual(lower.at(0u), decrement_))
    {
      decrement(current_couplette_, words);
    }
    else if (rockEqual(lower.at(0u), put_))
    {
      put(current_couplette_, words);
    }
    else if (rockEqual(lower.at(0u), cout_))
    {
      cout(current_couplette_, words);
    }
    else if (rockEqual(lower.at(0u), cin_) && rockEqual(lower.at(1u), "to"))
    {
      cin(current_couplette_, words);
    }
    else if (rockEqual(lower.at(0u), give_) && rockEqual(lower.at(1u), "back"))
    {
      giveBack(words);
    }
    else // All other commands should be based on the first found word. Not first found type.
    {
      for (const String& word : lower)
      {
        if (rockEqual(word, plus_) || rockEqual(word, minus_) || rockEqual(word, multiply_) || rockEqual(word, divide_))
        {
          arithmetic(current_couplette_, words);
          break;
        }
        else if (rockEqual(word, says_))
        {
          says(current_couplette_, words);
          break;
        }
        else if (rockEqual(word, is_))
        {
          is(current_couplette_, words);
          break;
        }
        else if (rockEqual(word, takes_))
        {
          takes(words);
          break;
        }
        else if (rockEqual(word, taking_))
        {
          taking(words);
          break;
        }
      }
    }
  }
  Song Composer::compose(const String& file)
  {
    Couplette* root = newCouplette("root");
    Couplette* main = current_couplette_ = newCouplette("main");
    createDefaultVariables(root);
    root->children.push_back(main);
    main->parent = root;

    Vector<String> lines = rockSplit(file, '\n');
    size_t total_size = 0u;

    for (const String& line : lines)
    {
      total_size += line.size();
      lineToActions(line);
    }

    while (current_couplette_->parent != root)
    {
      lineToActions("");
    }

    return Song(generateSongName(total_size), generateArtistName(total_size), root);
  }

  void Composer::ifStatement(Couplette* couplette, const Vector<String>& line)
  {
    Vector<String> segments = line;
    segments.erase(segments.begin());
    segments = rockSplit(segments, "and");
    // TODO (Hilze): add support for and in functions.
    
    for (const String& segment : segments)
    {
      enum
      {
        kEqual,
        kNotEqual,
        kGreater,
        kLess,
        kGreaterEqual,
        kLessEqual
      } type = kEqual;

      Vector<String> words = rockSplit(segment, ' ');
      size_t idx = 0u;
      for (idx = 0u; idx < words.size(); ++idx)
      {
        if (rockEqual(words.at(idx), is_))
        {
          break;
        }
        else if (rockEqual(words.at(idx), "ain't"))
        {
          type = kNotEqual;
          break;
        }
      }

      String lhs = rockToString(words, 0u, idx);
      String rhs = rockToString(words, idx + 1u);


      if (rockEqual(words.at(idx + 1u), "not"))
      {
        if (type == kNotEqual)
        {
          type = kEqual;
        }
        else
        {
          type = kNotEqual;
        }
        rhs = rockToString(words, idx + 2u);
      }
      else if (rockEqual(words.at(idx + 1u), greater_))
      {
        type = kGreater;
        rhs = rockToString(words, idx + 3u);
      }
      else if (rockEqual(words.at(idx + 1u), less_))
      {
        type = kLess;
        rhs = rockToString(words, idx + 3u);
      }
      else if (rockEqual(words.at(idx + 1u), "as"))
      {
        if (rockEqual(words.at(idx + 2u), greater_equal_))
        {
          type = kGreaterEqual;
          rhs = rockToString(words, idx + 4u);
        }
        else if (rockEqual(words.at(idx + 2u), less_equal_))
        {
          type = kLessEqual;
          rhs = rockToString(words, idx + 4u);
        }
      }

      TypeContainer* lhs_type = subExecute(current_couplette_, lhs);
      TypeContainer* rhs_type = subExecute(current_couplette_, rhs);


      float f;
      if (rockIsFloat(lhs, &f))
      {
        lhs_type = newTemporary(current_couplette_);
        lhs_type->construct(rockConstruct<NumberType>());
        lhs_type->asNumber()->set(f);
      }
      if (rockIsFloat(rhs, &f))
      {
        rhs_type = newTemporary(current_couplette_);
        rhs_type->construct(rockConstruct<NumberType>());
        rhs_type->asNumber()->set(f);
      }

      if (lhs_type == nullptr) lhs_type = getType(current_couplette_, lhs);
      if (rhs_type == nullptr) rhs_type = getType(current_couplette_, rhs);

      BlockerContainer* blocker = rockConstruct<BlockerContainer>();

      switch (type)
      {
      case kEqual:
        blocker->construct(rockConstruct<EqualBlocker>());
        break;
      case kNotEqual:
        blocker->construct(rockConstruct<NotEqualBlocker>());
        break;
      case kGreater:
        blocker->construct(rockConstruct<GreaterThanBlocker>());
        break;
      case kLess:
        blocker->construct(rockConstruct<LessThanBlocker>());
        break;
      case kGreaterEqual:
        blocker->construct(rockConstruct<GreaterThanOrEqualToBlocker>());
        break;
      case kLessEqual:
        blocker->construct(rockConstruct<LessThanOrEqualToBlocker>());
        break;
      }
      
      blocker->asIBlocker()->setLhs(lhs_type);
      blocker->asIBlocker()->setRhs(rhs_type);
      couplette->blockers.push_back(blocker);
    }
  }

  void Composer::ifStatement(const Vector<String>& line)
  {
    Couplette* couplette = newCouplette("if");
    couplette->parent = current_couplette_;
    current_couplette_->children.push_back(couplette);

    TypeContainer* did_execute = getType(couplette, "__did_execute__");

    // Add if statement.
    ifStatement(couplette, line);

    // Assign hidden variable to false.
    assign(current_couplette_, did_execute, "false");
    assign(couplette, did_execute, "true");

    ActionContainer* action = newAction(current_couplette_);
    action->construct(rockConstruct<CoupletteAction>());
    action->asCouplette()->setCouplette(couplette);

    current_couplette_ = couplette;
  }

  void Composer::elseStatement(const Vector<String>& line)
  {
    if (line.size() >= 2u && line.at(1u) == "if")
    {
      // Create blocker.
      Vector<BlockerContainer*> blockers;

      for (int i = current_couplette_->children.size() - 1; i >= 0; --i)
      {
        blockers.push_back(rockConstruct<BlockerContainer>());
        blockers.back()->construct(rockConstruct<EqualBlocker>());
        blockers.back()->asIBlocker()->setLhs(getType(current_couplette_->children.at(i), "__did_execute__"));
        blockers.back()->asIBlocker()->setRhs(getType(current_couplette_, "false"));

        if (rockEqual(current_couplette_->children.at(i)->name, "if"))
        {
          break;
        }
      }

      // Add the new if statement.
      Vector<String> nwords = line;
      nwords.erase(nwords.begin());
      lineToActions(rockToString(nwords));

      // At blocker.
      current_couplette_->name = "else if";

      for (BlockerContainer* blocker : blockers)
      {
        current_couplette_->blockers.push_back(blocker);
      }
    }
    else
    {
      // Create the new blocker.
      Vector<BlockerContainer*> blockers;

      for (int i = current_couplette_->children.size() - 1; i >= 0; --i)
      {
        blockers.push_back(rockConstruct<BlockerContainer>());
        blockers.back()->construct(rockConstruct<EqualBlocker>());
        blockers.back()->asIBlocker()->setLhs(getType(current_couplette_->children.at(i), "__did_execute__"));
        blockers.back()->asIBlocker()->setRhs(getType(current_couplette_, "false"));

        if (rockEqual(current_couplette_->children.at(i)->name, "if"))
        {
          break;
        }
      }

      Couplette* couplette = newCouplette("else");

      couplette->parent = current_couplette_;
      current_couplette_->children.push_back(couplette);

      for (BlockerContainer* blocker : blockers)
      {
        couplette->blockers.push_back(blocker);
      }

      ActionContainer* action = newAction(current_couplette_);
      action->construct(rockConstruct<CoupletteAction>());
      action->asCouplette()->setCouplette(couplette);

      current_couplette_ = couplette;
    }
  }

  void Composer::whileStatement(const Vector<String>& line)
  {
    Couplette* couplette = newCouplette("while");
    couplette->parent = current_couplette_;
    current_couplette_->children.push_back(couplette);

    ifStatement(couplette, line);

    ActionContainer* action = newAction(current_couplette_);
    action->construct(rockConstruct<CoupletteAction>());
    action->asCouplette()->setCouplette(couplette);

    current_couplette_ = couplette;
  }

  void Composer::untilStatement(const Vector<String>& line)
  {
    Couplette* couplette = newCouplette("until");
    couplette->parent = current_couplette_;
    current_couplette_->children.push_back(couplette);

    size_t idx = 0u;
    for (idx = 0u; idx < line.size(); ++idx)
    {
      if (rockEqual(line.at(idx), is_))
      {
        break;
      }
    }

    String name  = rockToString(line, 1u, idx);
    String value = rockToString(line, idx + 1u);

    BlockerContainer* blocker = rockConstruct<BlockerContainer>();
    blocker->construct(rockConstruct<NotEqualBlocker>());
    blocker->asIBlocker()->setLhs(getType(couplette, name));
    blocker->asIBlocker()->setRhs(getType(couplette, value));

    couplette->blockers.push_back(blocker);

    ActionContainer* action = newAction(current_couplette_);
    action->construct(rockConstruct<CoupletteAction>());
    action->asCouplette()->setCouplette(couplette);
    
    current_couplette_ = couplette;
  }

  void Composer::takes(const Vector<String>& line)
  {
    size_t idx = 0u;
    for (idx = 0u; idx < line.size(); ++idx)
    {
      if (rockEqual(line.at(idx), takes_))
      {
        break;
      }
    }
    String name  = rockToString(line, 0u, idx);
    String value = rockToString(line, idx + 1u);

    Vector<String> inputs;
    String input;

    for (const String& word : rockSplit(value, ' '))
    {
      if (rockEqual(word, "and"))
      {
        if (false == input.empty())
        {
          inputs.push_back(input);
          input.clear();
        }
      }
      else
      {
        if (input.empty())
        {
          input += word;
        }
        else
        {
          input += " " + word;
        }
      }
    }
    if (false == input.empty())
    {
      inputs.push_back(input);
      input.clear();
    }

    Couplette* couplette = newCouplette(name);
    couplette->parent = getRoot();
    current_couplette_->children.push_back(couplette);

    for (size_t i = 0u; i < inputs.size(); ++i)
    {
      TypeContainer* temp_type = getType(couplette, "__input_" + rockToString(i) + "__");
      TypeContainer* type      = getType(couplette, inputs.at(i));
      
      ActionContainer* action = newAction(couplette);
      action->construct(rockConstruct<AssignAction>());
      action->asAssign()->setType(type);
      action->asAssign()->setAssign(temp_type);
    }

    current_couplette_ = couplette;
  }

  void Composer::taking(const Vector<String>& line)
  {
    size_t idx = 0u;
    for (idx = 0u; idx < line.size(); ++idx)
    {
      if (rockEqual(line.at(idx), taking_))
      {
        break;
      }
    }
    String name  = rockToString(line, 0u, idx);
    String value = rockToString(line, idx + 1u);

    Vector<String> parameters = rockSplit(value, ',');
    for (String& str : parameters)
    {
      if (str.at(0u) == ' ')
      {
        str.erase(str.begin());
      }
    }

    Couplette* couplette = findCouplette(current_couplette_, name);

    for (size_t i = 0u; i < parameters.size(); ++i)
    {
      ActionContainer* action = newAction(current_couplette_);
      action->construct(rockConstruct<AssignAction>());
      action->asAssign()->setType(getType(couplette, "__input_" + rockToString(i) + "__"));
      action->asAssign()->setAssign(getType(current_couplette_, parameters.at(i)));
    }

    ActionContainer* action = newAction(current_couplette_);
    action->construct(rockConstruct<CoupletteAction>());
    action->asCouplette()->setCouplette(couplette);

    ActionContainer* return_action = newAction(current_couplette_);
    return_action->construct(rockConstruct<AssignAction>());
    return_action->asAssign()->setAssign(getType(couplette, "__return__"));
    return_action->asAssign()->setType(newTemporary(current_couplette_));
  }

  void Composer::giveBack(const Vector<String>& line)
  {
    String name = rockToString(line, 2u);

    TypeContainer* value = getType(current_couplette_, name);
    TypeContainer* ret   = getType(current_couplette_, "__return__");
    
    ActionContainer* action = newAction(current_couplette_);
    action->construct(rockConstruct<AssignAction>());
    action->asAssign()->setAssign(value);
    action->asAssign()->setType(ret);
  }

  void Composer::increment(Couplette* couplette, const Vector<String>& line)
  {
    ActionContainer* action = newAction(couplette);
    action->construct(rockConstruct<IncrementAction>());
    action->asIAction()->setType(getType(couplette, rockToString(line, 1u, line.size() - 1u)));
  }
  void Composer::decrement(Couplette* couplette, const Vector<String>& line)
  {
    ActionContainer* action = newAction(couplette);
    action->construct(rockConstruct<DecrementAction>());
    action->asIAction()->setType(getType(couplette, rockToString(line, 1u, line.size() - 1u)));
  }
  void Composer::assign(Couplette* couplette, const String& name, const String& value)
  {
    assign(couplette, getType(couplette, name), value);
  }
  void Composer::assign(Couplette* couplette, TypeContainer* type, const String& value)
  {
    ActionContainer* action = rockConstruct<ActionContainer>();
    action->construct(rockConstruct<AssignAction>());
    action->asIAction()->setType(type);

    if (value.at(0u) == '\"')
    {
      TypeContainer* temporary = newTemporary(couplette);
      temporary->construct(rockConstruct<StringType>());
      temporary->asString()->set(value.substr(1u, value.size() - 2u));
      action->asAssign()->setAssign(temporary);
    }
    else if (rockEqual(value, true_))
    {
      TypeContainer* temporary = newTemporary(couplette);
      temporary->construct(rockConstruct<BoolType>());
      temporary->asBool()->set(true);
      action->asAssign()->setAssign(temporary);
    }
    else if (rockEqual(value, false_))
    {
      TypeContainer* temporary = newTemporary(couplette);
      temporary->construct(rockConstruct<BoolType>());
      temporary->asBool()->set(false);
      action->asAssign()->setAssign(temporary);
    }
    else if (rockEqual(value, null_))
    {
      action->asAssign()->setAssign(getType(couplette, value));
    }
    else
    {
      TypeContainer* result = nullptr;
      float f = 0.0f;
      if (rockIsFloat(value, &f))
      {
        result = newTemporary(couplette);
        result->construct(rockConstruct<NumberType>());
        result->asNumber()->set(f);
      }
      else if (hasType(couplette, value))
      {
        result = getType(couplette, value);
      }
      else
      {
        result = subExecute(couplette, value);
        if (result == nullptr)
        {
          result = newTemporary(couplette);
          result->construct(rockConstruct<NumberType>());

          float number = rockStringToNumber(value);

          result->asNumber()->set(number);
        }
      }

      action->asAssign()->setAssign(result);
    }
    couplette->chords.push_back(action);
  }
  void Composer::put(Couplette* couplette, const Vector<String>& line)
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
    assign(couplette, name, value);
  }
  void Composer::says(Couplette* couplette, const Vector<String>& line)
  {
    size_t idx = 0u;
    for (idx = 0u; idx < line.size(); ++idx)
    {
      if (rockEqual(line.at(idx), says_))
      {
        break;
      }
    }

    String name  = rockToString(line, 0u, idx);
    String value = rockToString(line, idx + 1u);
    ActionContainer* action = newAction(couplette);
    action->construct(rockConstruct<AssignAction>());
    action->asIAction()->setType(getType(couplette, name));

    TypeContainer* temporary = newTemporary(couplette);
    temporary->construct(rockConstruct<StringType>());
    temporary->asString()->set(value);
    action->asAssign()->setAssign(temporary);
  }
  void Composer::is(Couplette* couplette, const Vector<String>& line)
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
    assign(couplette, name, value);
  }

  void Composer::cout(Couplette* couplette, const Vector<String>& line)
  {
    String name = rockToString(line, 1u);

    TypeContainer* type;
    if (rockEqual(name.at(0u), '"'))
    {
      type = newTemporary(couplette);
      type->construct(rockConstruct<StringType>());
      type->asString()->set(name.substr(1u, name.size() - 2u));
    }
    else
    {
      type = getType(couplette, name);
    }

    ActionContainer* action = newAction(couplette);
    action->construct(rockConstruct<COutAction>());
    action->asIAction()->setType(type);
  }

  void Composer::cin(Couplette* couplette, const Vector<String>& line)
  {
    String name = rockToString(line, 2u);

    ActionContainer* action = newAction(couplette);
    action->construct(rockConstruct<CInAction>());
    action->asIAction()->setType(getType(couplette, name));
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
      }
      else if (rockEqual(word, minus_))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name, OpData::kNumber });
          name.clear();
        }
        operators.push_back(OpData{ word, OpData::kSubtract });
      }
      else if (rockEqual(word, multiply_))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name, OpData::kNumber });
          name.clear();
        }
        operators.push_back(OpData{ word, OpData::kMultiply });
      }
      else if (rockEqual(word, divide_))
      {
        if (false == name.empty())
        {
          operators.push_back(OpData{ name, OpData::kNumber });
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
  void Composer::createQueue(const Vector<OpData>& operators, const Vector<OpWeight>& weights, Couplette* couplette)
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
        lhs = getType(couplette, operators.at(weight.idx - 1u).val);
      }

      // Find out what the rhs should be.
      if (hasOpRight(operators, weight.idx) && types.find(weight.idx + 2u) != types.end())
      {
        rhs = types.at(weight.idx + 2u);
      }
      else
      {
        rhs = getType(couplette, operators.at(weight.idx + 1u).val);
      }

      // Create the new temporary value.
      TypeContainer* tmp = newTemporary(couplette);
      types[weight.idx] = tmp;

      // Create the action.
      ActionContainer* action = newAction(couplette);
      switch (op.type)
      {
      case OpData::kAdd:      action->construct(rockConstruct<PlusAction>());     break;
      case OpData::kSubtract: action->construct(rockConstruct<MinusAction>());    break;
      case OpData::kMultiply: action->construct(rockConstruct<MultiplyAction>()); break;
      case OpData::kDivide:   action->construct(rockConstruct<DivideAction>());   break;
      }
      action->asArithmetic()->setLeft(lhs);
      action->asArithmetic()->setRight(rhs);
      action->asArithmetic()->setType(tmp);
    }
  }
  void Composer::arithmetic(Couplette* couplette, const Vector<String>& line)
  {
    Vector<OpData> operators;
    Vector<OpWeight> weights;
    getOperators(line, operators);
    weightOperators(operators, weights);
    createQueue(operators, weights, couplette);
  }
  bool Composer::hasType(Couplette* couplette, const String& name) const
  {
    for (TypeContainer* type : couplette->notes)
    {
      if (rockEqual(type->getName(), name))
      {
        return true;
      }
    }
    if (couplette->parent != nullptr && hasType(couplette->parent, name))
    {
      return true;
    }

    return false;
  }

  TypeContainer* Composer::getType(Couplette* couplette, const String& name)
  {
    String n = rockToLower(name);

    for (TypeContainer* type : couplette->notes)
    {
      if (rockEqual(type->getName(), n))
      {
        return type;
      }
    }
    if (couplette->parent != nullptr && hasType(couplette->parent, n))
    {
      return getType(couplette->parent, n);
    }

    TypeContainer* var = rockConstruct<TypeContainer>();
    var->setName(n);
    couplette->notes.push_back(var);

    return getType(couplette, n);
  }
  TypeContainer* Composer::newTemporary(Couplette* couplette)
  {
    String str = "temporary_" + rockToString(temporary_variables_++);
    return getType(couplette, str);
  }
  TypeContainer* Composer::lastTemporary(Couplette* couplette)
  {
    return getTemporary(couplette, temporary_variables_ - 1u);
  }
  TypeContainer* Composer::getTemporary(Couplette* couplette, size_t idx)
  {
    String str = "temporary_" + rockToString(idx);
    return getType(couplette, str);
  }

  TypeContainer* Composer::subExecute(Couplette* couplette, const String& value)
  {
    size_t size = couplette->chords.size();
    lineToActions(value);

    if (size != couplette->chords.size())
    {
      return lastTemporary(couplette);
    }
    else
    {
      return nullptr;
    }
  }

  Couplette* Composer::findCouplette(Couplette* couplette, const String& name)
  {
    if (rockEqual(couplette->name, name))
    {
      return couplette;
    }

    for (Couplette* child : couplette->children)
    {
      if (rockEqual(child->name, name))
      {
        return child;
      }
    }

    if (couplette->parent != nullptr)
    {
      return findCouplette(couplette->parent, name);
    }
    else
    {
      return nullptr;
    }
  }

  Couplette* Composer::getRoot()
  {
    Couplette* couplette = current_couplette_;
    while (couplette->parent != nullptr)
    {
      couplette = couplette->parent;
    }
    return couplette;
  }

  Couplette* Composer::newCouplette(const String& name)
  {
    Couplette* couplette = rockConstruct<Couplette>();
    couplette->name = name;    
    return couplette;
  }

  ActionContainer* Composer::newAction(Couplette* couplette)
  {
    ActionContainer* action = rockConstruct<ActionContainer>();
    couplette->chords.push_back(action);

    return action;
  }

  void Composer::createDefaultVariables(Couplette* couplette)
  {
    for (const String& null : null_)
    {
      TypeContainer* null_container = rockConstruct<TypeContainer>();
      null_container->construct(rockConstruct<NullType>());
      null_container->setName(null);
      couplette->notes.push_back(null_container);
    }
    for (const String& boolean : true_)
    {
      TypeContainer* boolean_container = rockConstruct<TypeContainer>();
      boolean_container->construct(rockConstruct<BoolType>());
      boolean_container->setName(boolean);
      boolean_container->asBool()->set(true);
      couplette->notes.push_back(boolean_container);
    }
    for (const String& boolean : false_)
    {
      TypeContainer* boolean_container = rockConstruct<TypeContainer>();
      boolean_container->construct(rockConstruct<BoolType>());
      boolean_container->setName(boolean);
      boolean_container->asBool()->set(false);
      couplette->notes.push_back(boolean_container);
    }
  }
}
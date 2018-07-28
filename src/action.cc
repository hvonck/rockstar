#pragma once
#include "action.h"
#include "song.h"

namespace lambda
{
  String actionToString(const Actions& action)
  {
    switch (action)
    {
    default:
    case Actions::kMysterious: return "misterious";
    case Actions::kIncrement:  return "increment";
    case Actions::kDecrement:  return "decrement";
    case Actions::kAssign:     return "assign";
    case Actions::kPlus:       return "plus";
    case Actions::kMinus:      return "minus";
    case Actions::kMultiply:   return "multiply";
    case Actions::kDivide:     return "divide";
    case Actions::kCOut:       return "cout";
    case Actions::kCIn:        return "cin";
    }
  }

  template<typename T>
  void makeType(TypeContainer* container)
  {
    container->construct(rockConstruct<T>());
  }
  template<typename T>
  void makeIfMysterious(TypeContainer* container)
  {
    if (container->getType() == Types::kMysterious)
    {
      makeType<T>(container);
    }
  }
  
  void makeOfType(TypeContainer* container, Types type)
  {
    if (container->getType() == type)
    {
      return;
    }

    // Null
    if (container->getType() == Types::kNull && type == Types::kBool)
    {
      //rockDestruct(container->asNull());
      container->construct(rockConstruct<BoolType>());
      container->asBool()->set(false);
    }
    else if (container->getType() == Types::kNull && type == Types::kNumber)
    {
      //rockDestruct(container->asNull());
      container->construct(rockConstruct<NumberType>());
      container->asNumber()->set(0.0f);
    }
    else if (container->getType() == Types::kNull && type == Types::kString)
    {
      //rockDestruct(container->asNull());
      container->construct(rockConstruct<StringType>());
      container->asString()->set("");
    }

    // Bool
    else if (container->getType() == Types::kBool && type == Types::kNull)
    {
      //rockDestruct(container->asBool());
      container->construct(rockConstruct<NullType>());
    }
    else if (container->getType() == Types::kBool && type == Types::kNumber)
    {
      bool boolean = container->asBool()->get();
      //rockDestruct(container->asBool());
      container->construct(rockConstruct<NumberType>());
      container->asNumber()->set(boolean ? 1.0f : 0.0f);
    }
    else if (container->getType() == Types::kBool && type == Types::kString)
    {
      bool boolean = container->asBool()->get();
      //rockDestruct(container->asBool());
      container->construct(rockConstruct<StringType>());
      container->asString()->set(boolean ? "true" : "false");
    }

    // Number
    else if (container->getType() == Types::kNumber && type == Types::kNull)
    {
      //rockDestruct(container->asNumber());
      container->construct(rockConstruct<NullType>());
    }
    else if (container->getType() == Types::kNumber && type == Types::kBool)
    {
      float number = container->asNumber()->get();
      //rockDestruct(container->asNumber());
      container->construct(rockConstruct<BoolType>());
      container->asBool()->set(number == 0.0f ? false : true);
    }
    else if (container->getType() == Types::kNumber && type == Types::kString)
    {
      float number = container->asNumber()->get();
      //rockDestruct(container->asNumber());
      container->construct(rockConstruct<StringType>());
      container->asString()->set(rockToString((size_t)number));
    }

    // String
    // TODO (Hilze): Implement this stuff.
    else if (container->getType() == Types::kString && type == Types::kNull)
    {
      rockDestruct(container->asString());
      container->construct(rockConstruct<NullType>());
    }
    else if (container->getType() == Types::kString && type == Types::kBool)
    {
      rockDestruct(container->asString());
      container->construct(rockConstruct<BoolType>());
    }
    else if (container->getType() == Types::kString && type == Types::kNumber)
    {
      rockDestruct(container->asString());
      container->construct(rockConstruct<StringType>());
    }
  }
  
  void IncrementAction::play()
  {
    makeIfMysterious<NumberType>(type_);
    makeOfType(type_, Types::kNumber);
    type_->asNumber()->set(type_->asNumber()->get() + 1.0f);
  }
  void DecrementAction::play()
  {
    makeIfMysterious<NumberType>(type_);
    makeOfType(type_, Types::kNumber);
    type_->asNumber()->set(type_->asNumber()->get() - 1.0f);
  }
  void AssignAction::play()
  {
    if (assign_->getType() == Types::kBool)
    {
      assignBool();
    }
    else if (assign_->getType() == Types::kNumber)
    {
      assignNumber();
    }
    else if (assign_->getType() == Types::kString)
    {
      assignString();
    }
    else
    {
      assignNull();
    }
  }
  void AssignAction::assignNull()
  {
    makeIfMysterious<NullType>(type_);
    makeOfType(type_, Types::kNull);
    rockAssert(type_->getType() == Types::kNull, "variable was not null");
  }
  void AssignAction::assignBool()
  {
    makeIfMysterious<BoolType>(type_);
    rockAssert(type_->getType() == Types::kBool, "variable was not a boolean");
    rockAssert(assign_->getType() == Types::kBool, "assign was not a boolean");
    type_->asBool()->set(assign_->asBool()->get());
  }
  void AssignAction::assignNumber()
  {
    makeIfMysterious<NumberType>(type_);
    rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
    rockAssert(assign_->getType() == Types::kNumber, "assign was not a number");
    type_->asNumber()->set(assign_->asNumber()->get());
  }
  void AssignAction::assignString()
  {
    makeIfMysterious<StringType>(type_);
    rockAssert(type_->getType() == Types::kString, "variable was not a string");
    rockAssert(assign_->getType() == Types::kString, "assign was not a string");
    type_->asString()->set(assign_->asString()->get());
  }
  void PlusAction::play()
  {
    makeIfMysterious<NumberType>(type_);
    rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
    rockAssert(left_->getType() == Types::kNumber || left_->getType() == Types::kNull, "left was not a number or null");
    rockAssert(right_->getType() == Types::kNumber || right_->getType() == Types::kNull, "right was not a number or null");

    const float val_left = left_->getType() == Types::kNull ? 0.0f : left_->asNumber()->get();
    const float val_right = right_->getType() == Types::kNull ? 0.0f : right_->asNumber()->get();
    type_->asNumber()->set(val_left + val_right);
  }
  void MinusAction::play()
  {
    makeIfMysterious<NumberType>(type_);
    rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
    rockAssert(left_->getType() == Types::kNumber || left_->getType() == Types::kNull, "left was not a number or null");
    rockAssert(right_->getType() == Types::kNumber || right_->getType() == Types::kNull, "right was not a number or null");

    const float val_left = left_->getType() == Types::kNull ? 0.0f : left_->asNumber()->get();
    const float val_right = right_->getType() == Types::kNull ? 0.0f : right_->asNumber()->get();
    type_->asNumber()->set(val_left - val_right);
  }
  void MultiplyAction::play()
  {
    makeIfMysterious<NumberType>(type_);
    rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
    rockAssert(left_->getType() == Types::kNumber || left_->getType() == Types::kNull, "left was not a number or null");
    rockAssert(right_->getType() == Types::kNumber || right_->getType() == Types::kNull, "right was not a number or null");

    const float val_left = left_->getType() == Types::kNull ? 0.0f : left_->asNumber()->get();
    const float val_right = right_->getType() == Types::kNull ? 0.0f : right_->asNumber()->get();
    type_->asNumber()->set(val_left * val_right);
  }
  void DivideAction::play()
  {
    makeIfMysterious<NumberType>(type_);
    rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
    rockAssert(left_->getType() == Types::kNumber || left_->getType() == Types::kNull, "left was not a number or null");
    rockAssert(right_->getType() == Types::kNumber || right_->getType() == Types::kNull, "right was not a number or null");

    const float val_left = left_->getType() == Types::kNull ? 0.0f : left_->asNumber()->get();
    const float val_right = right_->getType() == Types::kNull ? 0.0f : right_->asNumber()->get();
    type_->asNumber()->set(val_left / val_right);
  }
  void COutAction::play()
  {
    switch (type_->getType())
    {
    case Types::kNull:       std::printf("nothing\n"); break;
    case Types::kBool:       std::printf("%s\n", type_->asBool()->get() ? "true" : "false"); break;
    case Types::kNumber:     std::printf("%f\n", type_->asNumber()->get()); break;
    case Types::kString:     std::printf("%s\n", type_->asString()->get().c_str()); break;
    case Types::kMysterious: std::printf("mysterious\n"); break;
    default: rockAssert(false, "variable was too mysterious to shout"); break;
    }
  }
  void CInAction::play()
  {
    // TODO (Hilze): Properly implement this. Should probably be like put.
    String str;
    std::getline(std::cin, str);

    if (rockIsFloat(str))
    {
      float flt;
      rockIsFloat(str, &flt);
      makeIfMysterious<NumberType>(type_);
      rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
      type_->asNumber()->set(flt);
    }
    else if (rockIsBool(str))
    {
      bool bln;
      rockIsBool(str, &bln);
      makeIfMysterious<BoolType>(type_);
      rockAssert(type_->getType() == Types::kBool, "variable was not a boolean");
      type_->asBool()->set(bln);
    }
    else if (str.empty() == false && str.at(0u) == '\"')
    {
      makeIfMysterious<StringType>(type_);
      rockAssert(type_->getType() == Types::kString, "variable was not a string");
      type_->asString()->set(str.substr(1u, str.size() - 2u));
    }
    else
    {
      makeIfMysterious<NumberType>(type_);
      rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
      type_->asNumber()->set(rockStringToNumber(str));
    }
  }
  void CoupletteAction::play()
  {
    bool can_play = true;

    for (BlockerContainer* blocker : couplette_->blockers)
    {
      if (blocker->asIBlocker()->blocking())
      {
        can_play = false;
        break;
      }
    }

    if (true == can_play)
    {
      couplette_->play();
    }
  }
}
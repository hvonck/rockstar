#pragma once
#include "type.h"
#include "containers.h"
#include <iostream>
#include "rock_functions.h"

namespace lambda
{
  enum class Actions
  {
    kMysterious,
    kIncrement,
    kDecrement,
    kAssignBool,
    kAssignNumber,
    kAssignString,
    kIf,
    kPlus,
    kMinus,
    kMultiply,
    kDivide,
    kCOut,
    kCIn,
  };

  inline String actionToString(const Actions& action)
  {
    switch (action)
    {
    default:
    case Actions::kMysterious:   return "misterious";
    case Actions::kIncrement:    return "increment";
    case Actions::kDecrement:    return "decrement";
    case Actions::kAssignBool:   return "assign_bool";
    case Actions::kAssignNumber: return "assign_number";
    case Actions::kAssignString: return "assign_string";
    case Actions::kIf:           return "if";
    case Actions::kPlus:         return "plus";
    case Actions::kMinus:        return "minus";
    case Actions::kMultiply:     return "multiply";
    case Actions::kDivide:       return "divide";
    case Actions::kCOut:         return "cout";
    }
  }

  class IAction;
  class MysteriousAction;
  class IncrementAction;
  class DecrementAction;
  class AssignBoolAction;
  class AssignNumberAction;
  class AssignStringAction;
  class IfAction;
  class PlusAction;
  class MinusAction;
  class MultiplyAction;
  class DivideAction;
  class ArithmeticAction;
  class COutAction;
  class CInAction;

  class ActionContainer
  {
  public:
    void construct(MysteriousAction* data)   { data_ = (IAction*)data; action_ = Actions::kMysterious;   }
    void construct(IncrementAction* data)    { data_ = (IAction*)data; action_ = Actions::kIncrement;    }
    void construct(DecrementAction* data)    { data_ = (IAction*)data; action_ = Actions::kDecrement;    }
    void construct(AssignBoolAction* data)   { data_ = (IAction*)data; action_ = Actions::kAssignBool;   }
    void construct(AssignNumberAction* data) { data_ = (IAction*)data; action_ = Actions::kAssignNumber; }
    void construct(AssignStringAction* data) { data_ = (IAction*)data; action_ = Actions::kAssignString; }
    void construct(IfAction* data)           { data_ = (IAction*)data; action_ = Actions::kIf;           }
    void construct(PlusAction* data)         { data_ = (IAction*)data; action_ = Actions::kPlus;         }
    void construct(MinusAction* data)        { data_ = (IAction*)data; action_ = Actions::kMinus;        }
    void construct(MultiplyAction* data)     { data_ = (IAction*)data; action_ = Actions::kMultiply;     }
    void construct(DivideAction* data)       { data_ = (IAction*)data; action_ = Actions::kDivide;       }
    void construct(COutAction* data)         { data_ = (IAction*)data; action_ = Actions::kCOut;         }
    void construct(CInAction* data)          { data_ = (IAction*)data; action_ = Actions::kCIn;          }
    Actions             getAction()      const { return action_;                    }
    IAction*            asIAction()      const { return data_;                      }
    MysteriousAction*   asMysterious()   const { return (MysteriousAction*)data_;   }
    IncrementAction*    asIncrement()    const { return (IncrementAction*)data_;    }
    DecrementAction*    asDecrement()    const { return (DecrementAction*)data_;    }
    AssignBoolAction*   asAssignBool()   const { return (AssignBoolAction*)data_;   }
    AssignNumberAction* asAssignNumber() const { return (AssignNumberAction*)data_; }
    AssignStringAction* asAssignString() const { return (AssignStringAction*)data_; }
    IfAction*           asIf()           const { return (IfAction*)data_;           }
    PlusAction*         asPlus()         const { return (PlusAction*)data_;         }
    MinusAction*        asMinus()        const { return (MinusAction*)data_;        }
    MultiplyAction*     asMultiply()     const { return (MultiplyAction*)data_;     }
    DivideAction*       asDivide()       const { return (DivideAction*)data_;       }
    ArithmeticAction*   asArithmetic()   const { return (ArithmeticAction*)data_;   }
    COutAction*         asCOut()         const { return (COutAction*)data_;         }
    CInAction*          asCIn()          const { return (CInAction*)data_;          }

  private:
    Actions  action_ = Actions::kMysterious;
    IAction* data_   = nullptr;
  };

  template<typename T>
  void makeType(TypeContainer* container)
  {
    container->construct(container->getName(), rockConstruct<T>());
  }
  template<typename T>
  void makeIfMysterious(TypeContainer* container)
  {
    if (container->getType() == Types::kMysterious)
    {
      makeType<T>(container);
    }
  }

  class IAction
  {
  public:
    void setType(TypeContainer* type) { type_ = type; }
    virtual void play() {};

  protected:
    TypeContainer* type_;
  };
  class MysteriousAction : public IAction
  {
  public:
    virtual void play() {};

  private:
  };
  class IncrementAction : public IAction
  {
  public:
    virtual void play()
    {
      makeIfMysterious<NumberType>(type_);
      rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
      type_->asNumber()->set(type_->asNumber()->get() + 1.0f);
    }
  };
  class DecrementAction : public IAction
  {
  public:
    virtual void play() override
    {
      makeIfMysterious<NumberType>(type_);
      rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
      type_->asNumber()->set(type_->asNumber()->get() - 1.0f);
    }
  };
  class AssignBoolAction : public IAction
  {
  public:
    void setAssign(TypeContainer* assign) { assign_ = assign; }
    virtual void play() override
    {
      makeIfMysterious<BoolType>(type_);
      rockAssert(type_->getType()   == Types::kBool, "variable was not a boolean");
      rockAssert(assign_->getType() == Types::kBool, "assign was not a boolean");
      type_->asBool()->set(assign_->asBool()->get());
    }

  private:
    TypeContainer* assign_ = nullptr;
  };
  class AssignNumberAction : public IAction
  {
  public:
    void setAssign(TypeContainer* assign) { assign_ = assign; }
    virtual void play() override
    {
      makeIfMysterious<NumberType>(type_);
      rockAssert(type_->getType()   == Types::kNumber, "variable was not a number");
      rockAssert(assign_->getType() == Types::kNumber, "assign was not a number");
      type_->asNumber()->set(assign_->asNumber()->get());
    }

  private:
    TypeContainer* assign_ = nullptr;
  };
  class AssignStringAction : public IAction
  {
  public:
    void setAssign(TypeContainer* assign) { assign_ = assign; }
    virtual void play() override
    {
      makeIfMysterious<StringType>(type_);
      rockAssert(type_->getType()   == Types::kString, "variable was not a string");
      rockAssert(assign_->getType() == Types::kString, "assign was not a string");
      type_->asString()->set(assign_->asString()->get());
    }

  private:
    TypeContainer* assign_ = nullptr;
  };
  class IfAction : public IAction
  {
  public:
    void setExecute(const bool& play) { exec_ = play; }
    void queue(ActionContainer* action) { actions_.push_back(action); }
    virtual void play() override
    {
      if (exec_ == true)
      {
        for (ActionContainer* action : actions_)
        {
          action->asIAction()->play();
        }
      }
    }

  private:
    bool exec_ = false;
    Vector<ActionContainer*> actions_;
  };
  class ArithmeticAction : public IAction
  {
  public:
    void setLeft(TypeContainer* type)  { left_  = type; }
    void setRight(TypeContainer* type) { right_ = type; }
  
  protected:
    TypeContainer* left_  = nullptr;
    TypeContainer* right_ = nullptr;
  };
  class PlusAction : public ArithmeticAction
  {
  public:
    virtual void play() override
    {
      makeIfMysterious<NumberType>(type_);
      rockAssert(type_->getType()  == Types::kNumber, "variable was not a number");
      rockAssert(left_->getType()  == Types::kNumber || left_->getType()  == Types::kNull, "left was not a number or null");
      rockAssert(right_->getType() == Types::kNumber || right_->getType() == Types::kNull, "right was not a number or null");

      const float val_left  = left_->getType()  == Types::kNull ? 0.0f : left_->asNumber()->get();
      const float val_right = right_->getType() == Types::kNull ? 0.0f : right_->asNumber()->get();
      type_->asNumber()->set(val_left + val_right);
    }
  };
  class MinusAction : public ArithmeticAction
  {
  public:
    virtual void play() override
    {
      makeIfMysterious<NumberType>(type_);
      rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
      rockAssert(left_->getType() == Types::kNumber || left_->getType() == Types::kNull, "left was not a number or null");
      rockAssert(right_->getType() == Types::kNumber || right_->getType() == Types::kNull, "right was not a number or null");

      const float val_left  = left_->getType()  == Types::kNull ? 0.0f : left_->asNumber()->get();
      const float val_right = right_->getType() == Types::kNull ? 0.0f : right_->asNumber()->get();
      type_->asNumber()->set(val_left - val_right);
    }
  }; 
  class MultiplyAction : public ArithmeticAction
  {
  public:
    virtual void play() override
    {
      makeIfMysterious<NumberType>(type_);
      rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
      rockAssert(left_->getType() == Types::kNumber || left_->getType() == Types::kNull, "left was not a number or null");
      rockAssert(right_->getType() == Types::kNumber || right_->getType() == Types::kNull, "right was not a number or null");

      const float val_left  = left_->getType()  == Types::kNull ? 0.0f : left_->asNumber()->get();
      const float val_right = right_->getType() == Types::kNull ? 0.0f : right_->asNumber()->get();
      type_->asNumber()->set(val_left * val_right);
    }
  };
  class DivideAction : public ArithmeticAction
  {
  public:
    virtual void play() override
    {
      makeIfMysterious<NumberType>(type_);
      rockAssert(type_->getType() == Types::kNumber, "variable was not a number");
      rockAssert(left_->getType() == Types::kNumber || left_->getType() == Types::kNull, "left was not a number or null");
      rockAssert(right_->getType() == Types::kNumber || right_->getType() == Types::kNull, "right was not a number or null");

      const float val_left  = left_->getType()  == Types::kNull ? 0.0f : left_->asNumber()->get();
      const float val_right = right_->getType() == Types::kNull ? 0.0f : right_->asNumber()->get();
      type_->asNumber()->set(val_left / val_right);
    }
  };
  class COutAction : public IAction
  {
  public:
    virtual void play() override
    {
      switch (type_->getType())
      {
      case Types::kNull:   std::printf("nothing\n"); break;
      case Types::kBool:   std::printf("%s\n", type_->asBool()->get() ? "true" : "false"); break;
      case Types::kNumber: std::printf("%f\n", type_->asNumber()->get()); break;
      case Types::kString: std::printf("%s\n", type_->asString()->get().c_str()); break;
      default: rockAssert(false, "variable was too mysterious to shout"); break;
      }
    }
  };
  class CInAction : public IAction
  {
    virtual void play() override
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
      else if(str.empty() == false && str.at(0u) == '\"')
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
  };
}
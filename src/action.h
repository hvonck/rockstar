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
    kAssign,
    kPlus,
    kMinus,
    kMultiply,
    kDivide,
    kCOut,
    kCIn,
    kCouplette
  };

  extern String actionToString(const Actions& action);

  struct Couplette;

  class IAction;
  class MysteriousAction;
  class IncrementAction;
  class DecrementAction;
  class AssignAction;
  class PlusAction;
  class MinusAction;
  class MultiplyAction;
  class DivideAction;
  class ArithmeticAction;
  class COutAction;
  class CInAction;
  class CoupletteAction;

  class ActionContainer
  {
  public:
    void construct(MysteriousAction* data)   { data_ = (IAction*)data; action_ = Actions::kMysterious;   }
    void construct(IncrementAction* data)    { data_ = (IAction*)data; action_ = Actions::kIncrement;    }
    void construct(DecrementAction* data)    { data_ = (IAction*)data; action_ = Actions::kDecrement;    }
    void construct(AssignAction* data)       { data_ = (IAction*)data; action_ = Actions::kAssign;       }
    void construct(PlusAction* data)         { data_ = (IAction*)data; action_ = Actions::kPlus;         }
    void construct(MinusAction* data)        { data_ = (IAction*)data; action_ = Actions::kMinus;        }
    void construct(MultiplyAction* data)     { data_ = (IAction*)data; action_ = Actions::kMultiply;     }
    void construct(DivideAction* data)       { data_ = (IAction*)data; action_ = Actions::kDivide;       }
    void construct(COutAction* data)         { data_ = (IAction*)data; action_ = Actions::kCOut;         }
    void construct(CInAction* data)          { data_ = (IAction*)data; action_ = Actions::kCIn;          }
    void construct(CoupletteAction* data)    { data_ = (IAction*)data; action_ = Actions::kCouplette;    }
    Actions             getAction()      const { return action_;                    }
    IAction*            asIAction()      const { return data_;                      }
    MysteriousAction*   asMysterious()   const { return (MysteriousAction*)data_;   }
    IncrementAction*    asIncrement()    const { return (IncrementAction*)data_;    }
    DecrementAction*    asDecrement()    const { return (DecrementAction*)data_;    }
    AssignAction*       asAssign()       const { return (AssignAction*)data_;       }
    PlusAction*         asPlus()         const { return (PlusAction*)data_;         }
    MinusAction*        asMinus()        const { return (MinusAction*)data_;        }
    MultiplyAction*     asMultiply()     const { return (MultiplyAction*)data_;     }
    DivideAction*       asDivide()       const { return (DivideAction*)data_;       }
    ArithmeticAction*   asArithmetic()   const { return (ArithmeticAction*)data_;   }
    COutAction*         asCOut()         const { return (COutAction*)data_;         }
    CInAction*          asCIn()          const { return (CInAction*)data_;          }
    CoupletteAction*    asCouplette()    const { return (CoupletteAction*)data_;    }

  private:
    Actions  action_ = Actions::kMysterious;
    IAction* data_   = nullptr;
  };

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
    virtual void play() override;
  };
  class DecrementAction : public IAction
  {
  public:
    virtual void play() override;
  };
  class AssignAction : public IAction
  {
  public:
    void setAssign(TypeContainer* assign) { assign_ = assign; }
    virtual void play() override;

  private:
    void assignBool();
    void assignNumber();
    void assignString();
    void assignNull();

  private:
    TypeContainer* assign_ = nullptr;
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
    virtual void play() override;
  };
  class MinusAction : public ArithmeticAction
  {
  public:
    virtual void play() override;
  }; 
  class MultiplyAction : public ArithmeticAction
  {
  public:
    virtual void play() override;
  };
  class DivideAction : public ArithmeticAction
  {
  public:
    virtual void play() override;
  };
  class COutAction : public IAction
  {
  public:
    virtual void play() override;
  };
  class CInAction : public IAction
  {
  public:
    virtual void play() override;
  };
  class CoupletteAction : public IAction
  {
  public:
    void setCouplette(Couplette* couplette) { couplette_ = couplette; }
    virtual void play() override;

  private:
    Couplette* couplette_ = nullptr;
  };
}
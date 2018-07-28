#pragma once
#include "containers.h"
#include "type.h"
#include "rock_functions.h"

namespace lambda
{
  enum class Blockers
  {
    kMysterious,
    kAlways,
    kEqual,
    kNotEqual,
    kGreaterThan,
    kLessThan,
    kGreaterThanOrEqualTo,
    kLessThanOrEqualTo,
  };

  inline String actionToString(const Blockers& blocker)
  {
    switch (blocker)
    {
    default:
    case Blockers::kMysterious:           return "misterious";
    case Blockers::kAlways:               return "always";
    case Blockers::kEqual:                return "equal";
    case Blockers::kNotEqual:             return "not equal";
    case Blockers::kGreaterThan:          return "greater than";
    case Blockers::kLessThan:             return "less than";
    case Blockers::kGreaterThanOrEqualTo: return "greater or equal than";
    case Blockers::kLessThanOrEqualTo:    return "less or equal than";
    }
  }

  class IBlocker;
  class MysteriousBlocker;
  class AlwaysBlocker;
  class EqualBlocker;
  class NotEqualBlocker;
  class GreaterThanBlocker;
  class LessThanBlocker;
  class GreaterThanOrEqualToBlocker;
  class LessThanOrEqualToBlocker;

  class BlockerContainer
  {
  public:
    void construct(MysteriousBlocker* data)           { data_ = (IBlocker*)data; blocker_ = Blockers::kMysterious;           }
    void construct(AlwaysBlocker* data)               { data_ = (IBlocker*)data; blocker_ = Blockers::kAlways;               }
    void construct(EqualBlocker* data)                { data_ = (IBlocker*)data; blocker_ = Blockers::kEqual;                }
    void construct(NotEqualBlocker* data)             { data_ = (IBlocker*)data; blocker_ = Blockers::kNotEqual;             }
    void construct(GreaterThanBlocker* data)          { data_ = (IBlocker*)data; blocker_ = Blockers::kGreaterThan;          }
    void construct(LessThanBlocker* data)             { data_ = (IBlocker*)data; blocker_ = Blockers::kLessThan;             }
    void construct(GreaterThanOrEqualToBlocker* data) { data_ = (IBlocker*)data; blocker_ = Blockers::kGreaterThanOrEqualTo; }
    void construct(LessThanOrEqualToBlocker* data)    { data_ = (IBlocker*)data; blocker_ = Blockers::kLessThanOrEqualTo;    }
    Blockers                     getBlocker()             const { return blocker_;                            }
    IBlocker*                    asIBlocker()             const { return data_;                               }
    MysteriousBlocker*           asMysterious()           const { return (MysteriousBlocker*)data_;           }
    AlwaysBlocker*               asAlways()               const { return (AlwaysBlocker*)data_;               }
    EqualBlocker*                asEqual()                const { return (EqualBlocker*)data_;                }
    NotEqualBlocker*             asNotEqual()             const { return (NotEqualBlocker*)data_;             }
    GreaterThanBlocker*          asGreaterThan()          const { return (GreaterThanBlocker*)data_;          }
    LessThanBlocker*             asLessThan()             const { return (LessThanBlocker*)data_;             }
    GreaterThanOrEqualToBlocker* asGreaterThanOrEqualTo() const { return (GreaterThanOrEqualToBlocker*)data_; }
    LessThanOrEqualToBlocker*    asLessThanOrEqualTo()    const { return (LessThanOrEqualToBlocker*)data_;    }

  private:
    Blockers blocker_ = Blockers::kMysterious;
    IBlocker* data_ = nullptr;
  };

  class IBlocker
  {
  public:
    virtual bool blocking() = 0;

    void setLhs(TypeContainer* lhs) { lhs_ = lhs; }
    void setRhs(TypeContainer* rhs) { rhs_ = rhs; }

  protected:
    TypeContainer* lhs_ = nullptr;
    TypeContainer* rhs_ = nullptr;
  };
  class AlwaysBlocker : public IBlocker
  {
  public:
    virtual bool blocking() override
    {
      return false;
    }
  private:
  };
  enum class BlockerDiff
  {
    kEqual,
    kNotEqual,
    kLessThan,
    kGreaterThan,
    kUnknown
  };
  inline BlockerDiff blockerDiff(TypeContainer* lhs_container, TypeContainer* rhs_container)
  {
    if (lhs_container == nullptr || rhs_container == nullptr)
    {
      return BlockerDiff::kUnknown;
    }

    int lhs_type = -1, rhs_type = -1;
    switch (lhs_container->getType())
    {
    case Types::kNull:   lhs_type = 0; break;
    case Types::kNumber: lhs_type = 1; break;
    case Types::kBool:   lhs_type = 2; break;
    case Types::kString: lhs_type = 3; break;
    }
    switch (rhs_container->getType())
    {
    case Types::kNull:   rhs_type = 0; break;
    case Types::kNumber: rhs_type = 1; break;
    case Types::kBool:   rhs_type = 2; break;
    case Types::kString: rhs_type = 3; break;
    }
    rockAssert(lhs_type >= 0 && rhs_type >= 0, "either lhs or rhs was unknown");

    BlockerDiff diff;
    // Numbers
    if (lhs_type == 1 && (rhs_type == 1 || rhs_type == 0))
    {
      float lhs = lhs_container->asNumber()->get();
      float rhs = (rhs_type == 1) ? rhs_container->asNumber()->get() : 0.0f;
      
      if      (lhs == rhs) diff = BlockerDiff::kEqual;
      else if (lhs >  rhs) diff = BlockerDiff::kGreaterThan;
      else if (lhs <  rhs) diff = BlockerDiff::kLessThan;
      else                 diff = BlockerDiff::kNotEqual;
    }
    else if (rhs_type == 1 && (lhs_type == 1 || lhs_type == 0))
    {
      float lhs = (lhs_type == 1) ? lhs_container->asNumber()->get() : 0.0f;
      float rhs = rhs_container->asNumber()->get();

      if      (lhs == rhs) diff = BlockerDiff::kEqual;
      else if (lhs >  rhs) diff = BlockerDiff::kGreaterThan;
      else if (lhs <  rhs) diff = BlockerDiff::kLessThan;
      else                 diff = BlockerDiff::kNotEqual;
    }
    // Booleans
    else if (lhs_type == 2 && (rhs_type == 2 || rhs_type == 0))
    {
      bool lhs = lhs_container->asBool()->get();
      bool rhs = (rhs_type == 2) ? rhs_container->asBool()->get() : false;

      if (lhs == rhs) diff = BlockerDiff::kEqual;
      else            diff = BlockerDiff::kNotEqual;
    }
    else if (rhs_type == 2 && (lhs_type == 2 || lhs_type == 0))
    {
      bool lhs = (lhs_type == 2) ? lhs_container->asBool()->get() : false;
      bool rhs = rhs_container->asBool()->get();

      if (lhs == rhs) diff = BlockerDiff::kEqual;
      else            diff = BlockerDiff::kNotEqual;
    }
    else if (lhs_type == 3 && (rhs_type == 3 || rhs_type == 0))
    {
      String lhs = lhs_container->asString()->get();
      String rhs = (rhs_type == 3) ? rhs_container->asString()->get() : "";

      if (rockEqual(lhs, rhs)) diff = BlockerDiff::kEqual;
      else                     diff = BlockerDiff::kNotEqual;
    }
    else if (rhs_type == 3 && (lhs_type == 3 || lhs_type == 0))
    {
      String lhs = (lhs_type == 3) ? lhs_container->asString()->get() : "";
      String rhs = rhs_container->asString()->get();

      if (rockEqual(lhs, rhs)) diff = BlockerDiff::kEqual;
      else                     diff = BlockerDiff::kNotEqual;
    }
    else if (lhs_type == 0 && lhs_type == 0)
    {
      diff = BlockerDiff::kEqual;
    }
    else
    {
      diff = BlockerDiff::kUnknown;
    }

    return diff;
  }
  class EqualBlocker : public IBlocker
  {
  public:
    virtual bool blocking() override
    {
      return !(blockerDiff(lhs_, rhs_) == BlockerDiff::kEqual);
    }
  };
  class NotEqualBlocker : public EqualBlocker
  {
    virtual bool blocking() override
    {
      return !EqualBlocker::blocking();
    }
  };
  class GreaterThanBlocker : public IBlocker
  {
    virtual bool blocking() override
    {
      BlockerDiff diff = blockerDiff(lhs_, rhs_);
      return !(diff == BlockerDiff::kGreaterThan);
    }
  };
  class LessThanBlocker : public IBlocker
  {
    virtual bool blocking() override
    {
      BlockerDiff diff = blockerDiff(lhs_, rhs_);
      return !(diff == BlockerDiff::kLessThan);
    }
  };
  class GreaterThanOrEqualToBlocker : public IBlocker
  {
    virtual bool blocking() override
    {
      BlockerDiff diff = blockerDiff(lhs_, rhs_);
      return !(diff == BlockerDiff::kEqual || diff == BlockerDiff::kGreaterThan);
    }
  };
  class LessThanOrEqualToBlocker : public IBlocker
  {
    virtual bool blocking() override
    {
      BlockerDiff diff = blockerDiff(lhs_, rhs_);
      return !(diff == BlockerDiff::kEqual || diff == BlockerDiff::kLessThan);
    }
  };
}
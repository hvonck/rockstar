#pragma once
#include "containers.h"
#include "action.h"
#include "type.h"
#include "song.h"

namespace lambda
{
  class Composer
  {
  private:
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

      OpData(const String& val, const Type& type) :
        val(val), 
        type(type) 
      {}
      String val;
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

  public:
    Composer();
    
    Song compose(const String& file);

  private:
    Vector<String> lineToWords(const String& line);
    void lineToActions(const String& line);

    void createBlocker(Couplette* couplette, const String& line, bool invert_equal);
    void ifStatement(Couplette* couplette, const Vector<String>& line);
    void ifStatement(const Vector<String>& line);
    void elseStatement(const Vector<String>& line);
    void whileStatement(const Vector<String>& line);
    void untilStatement(const Vector<String>& line);

    void takes(const Vector<String>& line);
    void taking(const Vector<String>& line);
    void giveBack(const Vector<String>& line);

    void increment(Couplette* couplette, const Vector<String>& line);
    void decrement(Couplette* couplette, const Vector<String>& line);
    void assign(Couplette* couplette, const String& name, const String& value);
    void assign(Couplette* couplette, TypeContainer* type, const String& value);
    void put(Couplette* couplette, const Vector<String>& line);
    void says(Couplette* couplette, const Vector<String>& line);
    void is(Couplette* couplette, const Vector<String>& line);
    
    void cout(Couplette* couplette, const Vector<String>& line);
    void cin(Couplette* couplette, const Vector<String>& line);
  
    void getOperators(const Vector<String>& line, Vector<OpData>& operators);
    void weightOperators(const Vector<OpData>& operators, Vector<OpWeight>& weights);
    bool hasOpLeft(const Vector<OpData>& operators, const size_t& idx);
    bool hasOpRight(const Vector<OpData>& operators, const size_t& idx);
    void createQueue(const Vector<OpData>& operators, const Vector<OpWeight>& weights, Couplette* couplette);
    void arithmetic(Couplette* couplette, const Vector<String>& line);

  private:
    
    bool           hasType(Couplette* couplette, const String& name) const;
    TypeContainer* getType(Couplette* couplette, const String& name);
    TypeContainer* newTemporary(Couplette* couplette);
    TypeContainer* lastTemporary(Couplette* couplette);
    TypeContainer* getTemporary(Couplette* couplette, size_t idx);

    TypeContainer* subExecute(Couplette* couplette, const String& value);

    Couplette*       findCouplette(Couplette* couplette, const String& name);
    Couplette*       getRoot();
    Couplette*       newCouplette(const String& name);
    ActionContainer* newAction(Couplette* couplette);
    void             createDefaultVariables(Couplette* couplette);

  private:
    String generateArtistName(const size_t& hash) const;
    String generateSongName(const size_t& hash) const;

  private:
    size_t temporary_variables_ = 0u;
    Couplette* current_couplette_ = nullptr;
    Vector<String> pronouns_; // For most recently refered variable. TODO (Hilze): Implement.
    Vector<String> null_;
    Vector<String> true_;
    Vector<String> false_;
    Vector<String> plus_;
    Vector<String> minus_;
    Vector<String> multiply_;
    Vector<String> divide_;
    Vector<String> increment_;
    Vector<String> decrement_;
    Vector<String> put_;
    Vector<String> into_;
    Vector<String> is_;
    Vector<String> says_;
    Vector<String> cout_;
    Vector<String> cin_;
    Vector<String> if_;
    Vector<String> else_;
    Vector<String> while_;
    Vector<String> until_;
    Vector<String> greater_;
    Vector<String> less_;
    Vector<String> greater_equal_;
    Vector<String> less_equal_;
    Vector<String> takes_;
    Vector<String> taking_;
    Vector<String> give_;
  };
}
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
        case OpData::kMultiply: weight = (count - idx) + 100u; break;
        case OpData::kDivide:   weight = (count - idx) + 100u; break;
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
    void lineToActions(Vector<ActionContainer>& actions, const String& line);

    void increment(Vector<ActionContainer>& actions, const Vector<String>& line);
    void decrement(Vector<ActionContainer>& actions, const Vector<String>& line);
    void assign(Vector<ActionContainer>& actions, const String& name, const String& value);
    void put(Vector<ActionContainer>& actions, const Vector<String>& line);
    void is(Vector<ActionContainer>& actions, const Vector<String>& line);
    
    void cout(Vector<ActionContainer>& actions, const Vector<String>& line);
    void cin(Vector<ActionContainer>& actions, const Vector<String>& line);
  
    void getOperators(const Vector<String>& line, Vector<OpData>& operators);
    void weightOperators(const Vector<OpData>& operators, Vector<OpWeight>& weights);
    bool hasOpLeft(const Vector<OpData>& operators, const size_t& idx);
    bool hasOpRight(const Vector<OpData>& operators, const size_t& idx);
    void createQueue(const Vector<OpData>& operators, const Vector<OpWeight>& weights, Vector<ActionContainer>& actions);
    void arithmetic(Vector<ActionContainer>& actions, const Vector<String>& line);

  private:
    
    bool           hasType(const String& name) const;
    TypeContainer* getType(const String& name);
    TypeContainer* newTemporary();
    TypeContainer* lastTemporary();
    TypeContainer* getTemporary(size_t idx);

    TypeContainer* subExecute(Vector<ActionContainer>& actions, const String& value);

  private:
    String generateArtistName(const size_t& hash) const;
    String generateSongName(const size_t& hash) const;

  private:
    Vector<String> last_rhs_;
    size_t temporary_variables_ = 0u;
    Vector<TypeContainer*>  variables_;
    Vector<ActionContainer> actions_;
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
    Vector<String> cout_;
    Vector<String> cin_;
  };
}
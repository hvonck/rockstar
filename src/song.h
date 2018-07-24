#pragma once
#include "type.h"
#include "action.h"

namespace lambda
{
  class Song
  {
  public:
    Song(const String& name, const String& artist, const Vector<TypeContainer*>& notes, const Vector<ActionContainer>& chords);
    void play();
    void output() const;

    TypeContainer* getNote(const String& name) const;
    const Vector<TypeContainer*>& getNotes() const;
    const Vector<ActionContainer>& getChords() const;

  protected:
    String                  name_;
    String                  artist_;
    Vector<TypeContainer*>  notes_;
    Vector<ActionContainer> chords_;
  };
}
#pragma once
#include "type.h"
#include "action.h"
#include "blocker.h"

namespace lambda
{
  struct Couplette
  {
    String name;
    Couplette* parent = nullptr;
    Vector<Couplette*> children;
 

    Vector<BlockerContainer*> blockers;
    Vector<TypeContainer*>    notes;
    Vector<ActionContainer*>  chords;

    void play();
  };

  class Song
  {
  public:
    Song(const String& name, const String& artist, Couplette* couplettes);
    void play();
    void output() const;

    TypeContainer* getNote(const String& name) const;
    const Vector<TypeContainer*>& getNotes() const;

    Couplette* getCouplette(const String& name) const;

  private:
    Couplette* getCouplette(Couplette* c, const String& name) const;

  protected:
    String     name_;
    String     artist_;
    Couplette* couplettes_;
  };
}
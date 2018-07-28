#include "song.h"
#include <iostream>
#include "rock_functions.h"

namespace lambda
{
  void Couplette::play()
  {
    for (ActionContainer* chord : chords)
    {
      chord->asIAction()->play();
    }
  }

  Song::Song(const String& name, const String& artist, Couplette* couplettes) :
    name_(name), artist_(artist), couplettes_(couplettes)
  {
  }
  void Song::play()
  {
    std::cout << "playing:\t" << name_ << "\nby:\t\t" << artist_ << "\n\n";
    getCouplette("main")->play();
  }
  void Song::output() const
  {
    for (auto note : getCouplette("main")->notes)
    {
      std::cout << note->getName() << ": ";
      switch (note->getType())
      {
      default:
      case Types::kMysterious: std::cout << "mysterious" << std::endl; break;
      case Types::kNull:       std::cout << "null" << std::endl; break;
      case Types::kBool:       std::cout << (note->asBool()->get() ? "true" : "false") << std::endl; break;
      case Types::kNumber:     std::cout << note->asNumber()->get() << std::endl; break;
      case Types::kString:     std::cout << note->asString()->get() << std::endl; break;
      case Types::kObject:     std::cout << "object" << std::endl; break;
      case Types::kFunction:   std::cout << "function" << std::endl; break;
      }
    }
  }
  TypeContainer* Song::getNote(const String& name) const
  {
    for (TypeContainer* note : getCouplette("main")->notes)
    {
      if (rockEqual(note->getName(), name))
      {
        return note;
      }
    }

    return nullptr;
  }
  const Vector<TypeContainer*>& Song::getNotes() const
  {
    return getCouplette("main")->notes;
  }
  Couplette* Song::getCouplette(const String& name) const
  {
    if (rockEqual(name, couplettes_->name))
    {
      return couplettes_;
    }
    else
    {
      return getCouplette(couplettes_, name);
    }
  }
  Couplette* Song::getCouplette(Couplette* c, const String & name) const
  {
    for (Couplette* couplette : c->children)
    {
      if (rockEqual(couplette->name, name))
      {
        return couplette;
      }
    }

    for (Couplette* couplette : c->children)
    {
      Couplette* cc = getCouplette(couplette, name);
      if (cc != nullptr)
      {
        return cc;
      }
    }

    return nullptr;
  }
}
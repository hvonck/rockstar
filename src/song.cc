#include "song.h"
#include <iostream>
#include "rock_functions.h"

namespace lambda
{
  Song::Song(const String& name, const String& artist, const Vector<TypeContainer*>& notes, const Vector<ActionContainer>& chords) :
    name_(name), artist_(artist), notes_(notes), chords_(chords)
  {
  }
  void Song::play()
  {
    std::cout << "playing:\t" << name_ << "\nby:\t\t" << artist_ << "\n\n";
    for (auto chord : chords_)
    {
      chord.asIAction()->play();
    }
  }
  void Song::output() const
  {
    for (auto note : notes_)
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
    for (TypeContainer* note : notes_)
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
    return notes_;
  }
  const Vector<ActionContainer>& Song::getChords() const
  {
    return chords_;
  }
}
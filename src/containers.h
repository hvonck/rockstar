#pragma once
#include <string>
#include <vector>
#include <map>
#include <cassert>

// This exists for easy intergration into my own engine.
// And/or any other engine/project which uses custom memory allocators and/or EASTL.
using String = std::string;
template<typename T>
using Vector = std::vector<T>;
template<typename K, typename V>
using Map    = std::map<K, V>;

#if !defined(ROCK_MALLOC)
#define ROCK_MALLOC
typedef void*(*iRockMalloc)(size_t);
typedef void(*iRockFree)(void*);
typedef void(*iRockAssert)(bool, const String&);

extern iRockMalloc rockMalloc;
extern iRockFree   rockFree;
extern iRockAssert rockAssert;

template<typename T>
T* rockConstruct()
{
  T* t = (T*)rockMalloc(sizeof(T));
  new(t) T();
  return t;
}
template<typename T>
void rockDestruct(const T* t)
{
  delete(t);
  rockFree((void*)t);
}
#endif
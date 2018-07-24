#include "containers.h"

void* defRockMalloc(size_t size)
{
  return std::malloc(size);
}
void defRockFree(void* data)
{
  std::free(data);
}
void defRockAssert(bool val, const String& msg)
{
  if (val == false)
  {
    printf("||==================||\n");
    printf("||An error occurred!||\n");
    printf("||==================||\n");
    printf("%s\n\n", msg.c_str());
    assert(false && "Error");
  }
}

iRockMalloc rockMalloc = defRockMalloc;
iRockFree   rockFree   = defRockFree;
iRockAssert rockAssert = defRockAssert;

#ifndef Attribute_Level_h__
#define Attribute_Level_h__

#include "Common.h"

template <class T>
struct Attribute_Level : public T {
  typedef Attribute_Level SelfType;
  ATTRIBUTE_COMMON(level)
  float level;

  Attribute_Level() :
    level(0)
    {}

  float GetLevel() const {
    return level;
  }

  bool HasLevel() const {
    return true;
  }
};

#endif

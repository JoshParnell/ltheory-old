#ifndef Attribute_Seed_h__
#define Attribute_Seed_h__

#include "Common.h"

template <class T>
struct Attribute_Seed : public T {
  typedef Attribute_Seed SelfType;
  ATTRIBUTE_COMMON(seed)
  uint seed;

  Attribute_Seed() :
    seed(0)
    {}

  uint const& GetSeed() const {
    return seed;
  }

  bool HasSeed() const {
    return true;
  }
};

#endif

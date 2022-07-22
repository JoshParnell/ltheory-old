#ifndef Attribute_Range_h__
#define Attribute_Range_h__

#include "Common.h"

/* TODO : Is merging going to break down here? How to handle range? */

template <class T>
struct Attribute_Range : public T {
  typedef Attribute_Range SelfType;
  ATTRIBUTE_COMMON(range)
  float range;

  Attribute_Range() :
    range(0)
    {}

  float const& GetRange() const {
    return range;
  }

  bool HasRange() const {
    return true;
  }
};

#endif

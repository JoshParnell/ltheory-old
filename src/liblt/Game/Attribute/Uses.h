#ifndef Attribute_Uses_h__
#define Attribute_Uses_h__

#include "Common.h"

template <class T>
struct Attribute_Uses : public T {
  typedef Attribute_Uses SelfType;
  ATTRIBUTE_COMMON(uses)
  Quantity uses;

  Attribute_Uses() :
    uses(0)
    {}

  Quantity GetUses() const {
    return uses;
  }

  bool HasUses() const {
    return true;
  }
};

#endif

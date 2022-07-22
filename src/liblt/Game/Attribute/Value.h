#ifndef Attribute_Value_h__
#define Attribute_Value_h__

#include "Common.h"

template <class T>
struct Attribute_Value : public T {
  typedef Attribute_Value SelfType;
  ATTRIBUTE_COMMON(value)
  Quantity value;

  Attribute_Value() :
    value(0)
    {}

  Quantity const& GetValue() const {
    return value;
  }

  bool HasValue() const {
    return true;
  }
};

#endif

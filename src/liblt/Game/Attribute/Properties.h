#ifndef Attribute_Properties_h__
#define Attribute_Properties_h__

#include "Common.h"

template <class T>
struct Attribute_Properties : public T {
  typedef Attribute_Properties SelfType;
  ATTRIBUTE_COMMON(properties)
  V3 properties;

  Attribute_Properties() :
    properties(0)
    {}

  V3 GetProperties() const {
    return properties;
  }

  bool HasProperties() const {
    return true;
  }
};

#endif

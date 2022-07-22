#ifndef Attribute_Object_h__
#define Attribute_Object_h__

#include "Common.h"

template <class T>
struct Attribute_Object : public T {
  typedef Attribute_Object SelfType;
  ATTRIBUTE_COMMON(object)
  Object object;

  Attribute_Object() {}

  Object const& GetObject() const {
    return object;
  }

  bool HasObject() const {
    return true;
  }
};

#endif

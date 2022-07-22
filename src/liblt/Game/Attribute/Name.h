#ifndef Attribute_Name_h__
#define Attribute_Name_h__

#include "Common.h"
#include "LTE/String.h"

template <class T>
struct Attribute_Name : public T {
  typedef Attribute_Name SelfType;
  ATTRIBUTE_COMMON(name)
  String name;

  String const& GetName() const {
    return name;
  }

  bool HasName() const {
    return true;
  }

  String ToString() const {
    return name;
  }
};

#endif

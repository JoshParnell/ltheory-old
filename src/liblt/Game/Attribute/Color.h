#ifndef Attribute_Color_h__
#define Attribute_Color_h__

#include "Common.h"
#include "LTE/Color.h"

template <class T>
struct Attribute_Color : public T {
  typedef Attribute_Color SelfType;
  ATTRIBUTE_COMMON(color)
  Color color;

  Attribute_Color() :
    color(0)
    {}

  Color const& GetColor() const {
    return color;
  }

  bool HasColor() const {
    return true;
  }
};

#endif

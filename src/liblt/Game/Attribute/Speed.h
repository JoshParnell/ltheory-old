#ifndef Attribute_Speed_h__
#define Attribute_Speed_h__

#include "Common.h"

template <class T>
struct Attribute_Speed : public T {
  typedef Attribute_Speed SelfType;
  ATTRIBUTE_COMMON(speed)
  float speed;

  Attribute_Speed() :
    speed(0)
    {}

  float const& GetSpeed() const {
    return speed;
  }

  bool HasSpeed() const {
    return true;
  }
};

#endif

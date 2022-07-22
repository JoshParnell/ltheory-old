#ifndef Attribute_Scale_h__
#define Attribute_Scale_h__

#include "Common.h"

template <class T>
struct Attribute_Scale : public T {
  typedef Attribute_Scale SelfType;
  ATTRIBUTE_COMMON(scale)
  float scale;

  Attribute_Scale() :
    scale(0)
    {}

  float const& GetScale() const {
    return scale;
  }

  bool HasScale() const {
    return true;
  }
};

#endif

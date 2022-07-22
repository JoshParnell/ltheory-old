#ifndef Attribute_Renderable_h__
#define Attribute_Renderable_h__

#include "Common.h"
#include "LTE/Renderable.h"

template <class T>
struct Attribute_Renderable : public T {
  typedef Attribute_Renderable SelfType;
  ATTRIBUTE_COMMON(renderable)
  Renderable renderable;

  Renderable const& GetRenderable() const {
    return renderable;
  }

  bool HasRenderable() const {
    return true;
  }
};

#endif

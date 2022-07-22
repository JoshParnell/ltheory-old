#ifndef Attribute_Traits_h__
#define Attribute_Traits_h__

#include "Common.h"
#include "AI/Traits.h"

template <class T>
struct Attribute_Traits : public T {
  typedef Attribute_Traits SelfType;
  ATTRIBUTE_COMMON(traits)
  Traits traits;

  Attribute_Traits() {}

  Traits const& GetTraits() const {
    return traits;
  }

  bool HasTraits() const {
    return true;
  }
};

#endif

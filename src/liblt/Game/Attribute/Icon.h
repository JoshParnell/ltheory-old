#ifndef Attribute_Icon_h__
#define Attribute_Icon_h__

#include "Common.h"
#include "UI/Icon.h"

template <class T>
struct Attribute_Icon : public T {
  typedef Attribute_Icon SelfType;
  ATTRIBUTE_COMMON(icon)
  Icon icon;

  Icon const& GetIcon() const {
    return icon;
  }

  bool HasIcon() const {
    return true;
  }
};

#endif

#ifndef Attribute_Metatype_h__
#define Attribute_Metatype_h__

#include "Common.h"
#include "LTE/Data.h"

template <class T>
struct Attribute_Metatype : public T {
  typedef Attribute_Metatype SelfType;
  ATTRIBUTE_COMMON(metatype)
  Data metatype;

  Attribute_Metatype() {}

  Data const& GetMetatype() const {
    return metatype;
  }

  bool HasMetatype() const {
    return true;
  }
};

#endif

#ifndef Attribute_Offset_h__
#define Attribute_Offset_h__

#include "Common.h"
#include "LTE/V3.h"

template <class T>
struct Attribute_Offset : public T {
  typedef Attribute_Offset SelfType;
  ATTRIBUTE_COMMON(offset)
  V3 offset;

  Attribute_Offset() :
    offset(0)
    {}

  V3 GetOffset() const {
    return offset;
  }

  bool HasOffset() const {
    return true;
  }
};

#endif

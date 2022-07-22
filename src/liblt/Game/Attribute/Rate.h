#ifndef Attribute_Rate_h__
#define Attribute_Rate_h__

#include "Common.h"

template <class T>
struct Attribute_Rate : public T {
  typedef Attribute_Rate SelfType;
  ATTRIBUTE_COMMON(rate)
  float rate;

  Attribute_Rate() :
    rate(0)
    {}

  float const& GetRate() const {
    return rate;
  }

  bool HasRate() const {
    return true;
  }
};

#endif

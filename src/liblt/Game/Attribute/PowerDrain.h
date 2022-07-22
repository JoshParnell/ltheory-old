#ifndef Attribute_PowerDrain_h__
#define Attribute_PowerDrain_h__

#include "Common.h"

template <class T>
struct Attribute_PowerDrain : public T {
  typedef Attribute_PowerDrain SelfType;
  ATTRIBUTE_COMMON(powerDrain)
  float powerDrain;

  float const& GetPowerDrain() const {
    return powerDrain;
  }

  bool HasPowerDrain() const {
    return true;
  }
};

#endif

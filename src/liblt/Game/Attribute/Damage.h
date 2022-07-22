#ifndef Attribute_Damage_h__
#define Attribute_Damage_h__

#include "Common.h"

template <class T>
struct Attribute_Damage : public T {
  typedef Attribute_Damage SelfType;
  ATTRIBUTE_COMMON(damage)
  Damage damage;

  Attribute_Damage() :
    damage(0)
    {}

  Damage GetDamage() const {
    return damage;
  }

  bool HasDamage() const {
    return true;
  }
};

#endif

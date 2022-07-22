#ifndef Attribute_Sound_h__
#define Attribute_Sound_h__

#include "Common.h"

template <class T>
struct Attribute_Sound : public T {
  typedef Attribute_Sound SelfType;
  ATTRIBUTE_COMMON(sound)
  String sound;

  String GetSound() const {
    return sound;
  }

  bool HasSound() const {
    return true;
  }
};

#endif

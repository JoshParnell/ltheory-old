#ifndef LTE_Quaternion_h__
#define LTE_Quaternion_h__

#include "V3.h"

namespace LTE {
  template <class T>
  class QuaternionT {
  public:
    V3T<T> axis;
    T angle;

    QuaternionT() {}

    QuaternionT(V3T<T> const& axis, T angle) :
      axis(axis),
      angle(angle)
      {}

    V4T<T> ToV4() const {
      return V4T<T>(axis, angle);
    }
  };
}

#endif

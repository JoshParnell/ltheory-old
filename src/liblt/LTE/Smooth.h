#ifndef LTE_Smooth_h__
#define LTE_Smooth_h__

#include "AutoClass.h"
#include "StdMath.h"

template <class T>
AutoClass(Smooth,
  T, value,
  T, target)

  Smooth() {}

  Smooth(T const& value) :
    value(value),
    target(value)
    {}

  operator T const& () const {
    return value;
  }

  Smooth& operator=(T const& value) {
    Set(value);
    return *this;
  }

  void Set(T const& valueTarget) {
    this->value = valueTarget;
    this->target = valueTarget;
  }

  void SetTarget(T const& target) {
    this->target = target;
  }

  void Update(double rate) {
    value = Mix(value, target, 1.0 - Exp(-rate));
  }

  void UpdateLinear(float rate) {
    T delta = target - value;
    T mag = Abs(delta);
    if (mag > 0) {
      delta *= Min(rate, mag) / mag;
      value += delta;
    }
  }
};

#endif

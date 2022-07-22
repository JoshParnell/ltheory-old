#ifndef LTE_Interpolated_h__
#define LTE_Interpolated_h__

#include "AutoClass.h"

namespace LTE {
  template <class T>
  AutoClass(Interpolated,
    T, last,
    T, current)

    Interpolated() {}

    Interpolated(T const& current) :
      last(current),
      current(current)
      {}

    template <class T2>
    Interpolated& operator+=(const T2& t2) {
      Update();
      current += t2;
      return *this;
    }

    template <class T2>
    Interpolated& operator-=(const T2& t2) {
      Update();
      current -= t2;
      return *this;
    }

    template <class T2>
    Interpolated& operator*=(const T2& t2) {
      Update();
      current *= t2;
      return *this;
    }

    template <class T2>
    Interpolated& operator/=(const T2& t2) {
      Update();
      current /= t2;
      return *this;
    }

    Interpolated& operator=(const T& t) {
      Update();
      current = t;
      return *this;
    }

    T operator()(float t) const {
      return (1 - t) * last + t * current;
    }

    void Update() {
      last = current;
    }
  };
}

#endif

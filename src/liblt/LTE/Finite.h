#ifndef LTE_Finite_h__
#define LTE_Finite_h__

#include "Common.h"

namespace LTE {
  template <class T>
  inline bool IsFinite(T const& t) {
    return true;
  }

  template <>
  inline bool IsFinite<float>(float const& t) {
    return t <= FLT_MAX && t >= -FLT_MAX;
  }

  template <>
  inline bool IsFinite<double>(double const& t) {
    return t <= DBL_MAX && t >= -DBL_MAX;
  }
}

#endif

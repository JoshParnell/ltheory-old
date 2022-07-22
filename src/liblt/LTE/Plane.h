#ifndef LTE_Plane_h__
#define LTE_Plane_h__

#include "AutoClass.h"
#include "V3.h"

template <class T>
AutoClassDerived(PlaneT, NullBase< PlaneT<T> >,
  V3T<T>, normal,
  T, w)

  PlaneT() {}

  PlaneT(V3T<T> const& origin, V3T<T> const& normal) :
    normal(Normalize(normal))
  {
    w = Dot(origin, this->normal);
  }

  static PlaneT FromPoints(
    V3T<T> const& p1,
    V3T<T> const& p2,
    V3T<T> const& p3)
  {
    return PlaneT(Cross(p2 - p1, p3 - p1), p1);
  }

  T PointTest(V3T<T> const& v) const {
    return Dot(v, normal) - w;
  }
};

#endif

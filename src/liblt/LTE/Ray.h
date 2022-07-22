#ifndef LTE_Ray_h__
#define LTE_Ray_h__

#include "AutoClass.h"
#include "V3.h"

#include <limits>

template <class PointT, class DirT>
AutoClass(RayT,
  PointT, origin,
  DirT, direction)

  typedef typename DirT::ReturnType LengthT;

  RayT() {}

  template <class P2, class D2>
  RayT(RayT<P2, D2> const& other) :
    origin((PointT)other.origin),
    direction((DirT)other.direction)
    {}

  static RayT FromPoints(PointT const& source, PointT const& dest) {
    return RayT(source, dest - source);
  }

  PointT operator()(LengthT t) const {
    return origin + t * direction;
  }

  LengthT GetDistance(PointT const& p) const {
    DirT toPoint = p - origin;
    return Length(toPoint - direction * (Dot(toPoint, direction)));
  }

  RayT GetNormalized() const {
    return RayT(origin, Normalize(direction));
  }

  bool IntersectsTri(
    PointT const& p0,
    PointT const& p1,
    PointT const& p2,
    LengthT& t) const
  {
    DirT e1 = p1 - p0;
    DirT e2 = p2 - p0;
    DirT s = origin - p0;
    DirT s1 = Cross(direction, e2);
    DirT s2 = Cross(s, e1);

    LengthT e1s1Dot = Dot(e1, s1);

    /* In theory, we need to handle this degenerate case.
       In practice, it destroys a lot of the robustness of my ray
       intersection routine...?? :( */
    //if (abs(e1s1Dot) < std::numeric_limits<T>::epsilon())
    //  return false;

    LengthT det = 1 / e1s1Dot;
    LengthT v = det * Dot(s1, s);
    LengthT w = det * Dot(s2, direction);
    t = det * Dot(s2, e2);
    LengthT u = 1 - v - w;
    return (t >= 0 &&
            u >= -std::numeric_limits<LengthT>::epsilon() &&
            v >= -std::numeric_limits<LengthT>::epsilon() &&
            w >= -std::numeric_limits<LengthT>::epsilon());
  }
};

#endif

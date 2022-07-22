#ifndef LTE_Capsule_h__
#define LTE_Capsule_h__

#include "Segment.h"

struct Capsule {
  Segment segment;
  float radius;

  Capsule(Segment const& segment, float radius) :
    segment(segment),
    radius(radius)
    {}
  
  float GetDistance(V3 const& p) const {
    return Max(0.f, segment.GetDistance(p) - radius);
  }
};

#endif

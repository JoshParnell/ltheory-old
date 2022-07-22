#ifndef LTE_Segment_h__
#define LTE_Segment_h__

#include "V3.h"

struct Segment {
  V3 origin;
  V3 axis;
  float length;

  Segment(V3 const& p1, V3 const& p2) : origin(p1) {
    axis = p2 - p1;
    length = Length(axis);
    axis /= length;
  }
  
  float GetDistance(V3 const& p) const {
    V3 toP = p - origin;
    float t = Clamp(Dot(toP, axis), 0.0f, length);
    return Length(toP - t * axis);
  }
};

#endif

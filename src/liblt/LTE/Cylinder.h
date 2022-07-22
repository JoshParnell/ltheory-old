#ifndef LTE_Cylinder_h__
#define LTE_Cylinder_h__

#include "AutoClass.h"
#include "Ray.h"
#include "V3.h"

AutoClass(Cylinder,
  V3, origin,
  V3, axis,
  float, radius)

  Cylinder() {}

  bool Itersects(Ray const& r, float* t = 0) const {
    V3 dirOP = OrthoProjection(ray.dir, axis);
    V3 dpOP = OrthoProjection(ray.origin - origin, axis);

    float a = LengthSquared(dirOP);
    float b = 2.0f * Dot(dirOP, dpOP);
    float c = LengthSquared(dpOP) - radius * radius;

    float disc = b * b - 4.0f * a * c;
    if (disc < 0)
      return false;

    disc = Sqrt(disc);

    /* Near. */ {
      float t1 = (-b - disc) / (2.0f * a);
      if (t1 >= 0) {
        if (t)
          *t = t1;
        return true;
      }
    }

    /* Far. */ {
      float t2 = (-b + disc) / (2.0f * a);
      if (t2 >= 0) {
        if (t)
          *t = t2;
        return true;
      }
    }

    return false;
  }
};

#endif

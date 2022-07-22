#ifndef LTE_Sphere_h__
#define LTE_Sphere_h__

#include "Ray.h"
#include "V3.h"

struct Sphere {
  V3 center;
  float radius;

  Sphere() :
    center(0),
    radius(1)
    {}

  Sphere(V3 const& center, float radius) :
    center(center),
    radius(radius)
    {}

  Sphere(V3 const& p1, V3 const& p2, V3 const& p3) {
    center = (p1 + p2 + p3) / 3.f;
    radius = Sqrt(Max(LengthSquared(center - p1),
                      LengthSquared(center - p2),
                      LengthSquared(center - p3)));
  }

  bool Contains(V3 const& p) const {
    float dx = center.x - p.x;
    float dy = center.y - p.y;
    float dz = center.z - p.z;
    dx *= dx;
    dy *= dy;
    dz *= dz;
    float r2 = radius*radius;

    /* Use early-rejection to speed up obviously-separated cases. */
    return dx <= r2 &&
           dy <= r2 &&
           dz <= r2 &&
           dx + dy + dz <= r2;
  }

  float GetVolume() const {
    return (4.0f / 3.0f) * kPi * (radius * radius * radius);
  }

  bool Intersects(Ray const& r) const {
    return Intersects(r.origin, Normalize(r.direction));
  }

  /* Faster version when the direction is guaranteed to be unit length. */
  bool Intersects(V3 const& rayOrigin, V3 const& rayDirectionUnit) const {
    V3 toCenter = center - rayOrigin;
    float d = Dot(toCenter, rayDirectionUnit);
    return LengthSquared(toCenter) - d*d <= radius * radius;
  }

  bool Intersects(Sphere const& s) {
    float totalRadius = radius + s.radius;
    float r2 = totalRadius * totalRadius;
    float dx = center.x - s.center.x;
    dx *= dx;
    if (dx > r2) return false;

    float dy = center.y - s.center.y;
    dy *= dy;
    if (dy > r2) return false;

    float dz = center.z - s.center.z;
    dz *= dz;
    if (dz > r2) return false;
    return dx + dy + dz <= r2;
  }
};

#endif

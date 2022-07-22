#ifndef LTE_Geometry_h__
#define LTE_Geometry_h__

#include "BaseType.h"
#include "Reference.h"

struct GeometryT : public RefCounted {
  BASE_TYPE(GeometryT)

  virtual void Draw() const = 0;

  virtual Bound3 GetBound() const = 0;

  virtual Mesh GetCollisionMesh() const = 0;

  virtual short GetVersion() const = 0;

  virtual bool Intersects(
    Ray const& r,
    float* tOut = nullptr,
    V3* normalOut = nullptr,
    V2* uvOut = nullptr) const = 0;

  virtual V3 Sample() const = 0;

  FIELDS {}
};

#endif

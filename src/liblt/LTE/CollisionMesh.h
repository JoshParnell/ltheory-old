#ifndef LTE_CollisionMesh_h__
#define LTE_CollisionMesh_h__

#include "Reference.h"

struct CollisionMeshT : public RefCounted {
  virtual ~CollisionMeshT() {}

  virtual size_t GetMemoryUsage() const = 0;

  virtual bool Intersects(
    CollisionMesh const& m,
    Matrix const& myWorld,
    Matrix const& otherWorld,
    V3* contactNormal = nullptr) const = 0;

  virtual bool Intersects(
    Ray const& ray,
    Matrix const& myWorld,
    float tMax,
    float& tOut,
    V3* normalOut = nullptr) const = 0;
};

LT_API CollisionMesh CollisionMesh_Create(Mesh const& sourceMesh);

#endif

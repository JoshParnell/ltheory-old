#ifndef LTE_PlateMesh_h__
#define LTE_PlateMesh_h__

#include "BaseType.h"
#include "DeclareFunction.h"
#include "Reference.h"
#include "V3.h"

struct PlateMeshT : public RefCounted {
  BASE_TYPE(PlateMeshT)

  virtual void Add(
    V3 const& center,
    V3 const& scale,
    V3 const& rotation,
    float bevel) = 0;

  virtual void Add(Warp const& warp) = 0;

  virtual void Add(
    PlateMesh const& child,
    V3 const& offset,
    V3 const& scale) = 0;

  virtual Mesh GetMesh() const = 0;

  virtual void ReflectX() = 0;
  virtual void ReflectY() = 0;
  virtual void ReflectZ() = 0;
};

DeclareFunction(PlateMesh_Create, PlateMesh,
  uint, quality)

DeclareFunction(Mesh_ComputeOcclusion, void,
  Mesh, mesh)

#endif

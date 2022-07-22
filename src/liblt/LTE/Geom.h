#ifndef LTE_Geom_h__
#define LTE_Geom_h__

#include "Vector.h"

namespace LTE {

  LT_API Mesh Geom_Box(float sx = 1, float sy = 1, float sz = 1);

  LT_API Mesh Geom_Cylinder(uint slices);

  LT_API Mesh Geom_DisplacePlane(Mesh const& m, Plane const& p, float amount);

  LT_API Mesh Geom_Expand(Mesh const& m, V3 const& origin, float x, float y, float z);

  LT_API Mesh Geom_MakeSymmetric(Mesh const& m, V3 const& plane);

  LT_API Mesh Geom_MakeSymmetricAngular(Mesh const& m, uint count, V3 const& axis);

  LT_API Mesh Geom_Mirror(Mesh const& m, V3 const& mirrorPlane);

  LT_API void Geom_MirrorPointRing(Vector<V3>& pointRing);

  LT_API Mesh Geom_PolyBox(uint sides, float sx, float sy, float sz);

  LT_API Mesh Geom_Repeat(
    Mesh const& m,
    uint count,
    V3 const& direction,
    float spacing = 1);

  LT_API Mesh Geom_Retarget(Mesh const& m, Bound3 const& targetBox);

  LT_API Mesh Geom_RingMesh(
    uint slices,
    uint pointCount,
    float bevelSize,
    float bevelDepth,
    float ringSpacing,
    float ringMinWidth,
    float ringMaxWidth,
    float edgeBevel,
    bool symmetricRing);

  LT_API Mesh Geom_SmoothHull(Mesh const& source, uint quality, float radius);

  LT_API Mesh Geom_Sphere(uint stacks, uint slices);

}

#endif

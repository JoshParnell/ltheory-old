#ifndef LTE_SDFMesh_h__
#define LTE_SDFMesh_h__

#include "SDF.h"

namespace LTE {
  LT_API Geometry SDFMesh_Create(
    SDF const& field,
    V3 const& resolutionMult = V3(1),
    float occlusionRadiusMult = 1.0f,
    float tileMult = 1.0f,
    bool eliminateFloaters = false);
}

#endif

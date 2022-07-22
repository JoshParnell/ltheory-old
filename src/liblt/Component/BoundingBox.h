#ifndef Component_BoundingBox_h__
#define Component_BoundingBox_h__

#include "Common.h"
#include "LTE/AutoClass.h"
#include "LTE/Bound.h"

AutoClass(ComponentBoundingBox,
  Bound3D, worldBox,
  Distance, radius,
  short, modelVersion,
  short, orientationVersion,
  short, queryVersion,
  short, inSpatialGrid)

  ComponentBoundingBox() :
    worldBox(0),
    radius(0),
    modelVersion(-1),
    orientationVersion(-1),
    queryVersion(-1),
    inSpatialGrid(-1)
    {}

  LT_API void Recompute(ObjectT const* self);
};

AutoComponent(BoundingBox)
};

#endif

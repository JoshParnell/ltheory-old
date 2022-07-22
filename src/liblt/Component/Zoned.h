#ifndef Component_Zoned_h__
#define Component_Zoned_h__

#include "Common.h"
#include "LTE/AutoClass.h"
#include "LTE/DeclareFunction.h"
#include "LTE/SDF.h"

AutoClass(ComponentZoned,
  SDF, region,
  float, fogDensity)

  ComponentZoned() : fogDensity(0) {}

  LT_API float GetContainment(ObjectT*, Position const&);
};

DeclareFunction(Object_GetZone, Object,
  Object, object)

AutoComponent(Zoned)
};

#endif

#ifndef LTE_Warps_h__
#define LTE_Warps_h__

#include "DeclareFunction.h"
#include "V3.h"
#include "Warp.h"

DeclareFunction(Warp_AttractorPlane, Warp,
  V3, center,
  V3, normal,
  float, strength)

DeclareFunction(Warp_AttractorPoint, Warp,
  V3, center,
  float, strength)

DeclareFunction(Warp_Custom, Warp,
  Data, data)

#endif

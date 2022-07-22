#ifndef UI_ClipRegion_h__
#define UI_ClipRegion_h__

#include "Common.h"
#include "LTE/DeclareFunction.h"
#include "LTE/V2.h"

DeclareFunctionNoParams(ClipRegion_GetMin, V2)
DeclareFunctionNoParams(ClipRegion_GetMax, V2)

DeclareFunctionNoParams(ClipRegion_Pop, void)

DeclareFunction(ClipRegion_Push, void,
  V2, pos,
  V2, size)

DeclareFunctionNoParams(ClipRegion_PushNoClip, void)

#endif

#ifndef Game_Icons_h__
#define Game_Icons_h__

#include "LTE/DeclareFunction.h"
#include "UI/Icon.h"

DeclareFunctionNoParams(Icon_Crosshair, Icon)

DeclareFunctionNoParams(Icon_Database, Icon)

DeclareFunctionNoParams(Icon_Disc, Icon)

DeclareFunctionNoParams(Icon_Dock, Icon)

DeclareFunction(Icon_Gear, Icon,
  float, width,
  uint, spokes,
  float, spokeWidth,
  float, spacing)

DeclareFunctionNoParams(Icon_Refresh, Icon)

DeclareFunctionNoParams(Icon_Task_Mine, Icon)

DeclareFunctionNoParams(Icon_Task_Produce, Icon)

DeclareFunctionNoParams(Icon_Task_Research, Icon)

#endif

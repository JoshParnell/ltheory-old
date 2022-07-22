#ifndef UI_Cursor_h__
#define UI_Cursor_h__

#include "LTE/DeclareFunction.h"
#include "LTE/V2.h"

DeclareFunctionNoParams(Cursor_Get, V2)
DeclareFunctionNoParams(Cursor_GetDelta, V2)
DeclareFunctionNoParams(Cursor_GetLast, V2)

DeclareFunctionNoParams(Cursor_Pop, void)

DeclareFunction(Cursor_Push, void,
  V2, pos,
  V2, posLast)

#endif

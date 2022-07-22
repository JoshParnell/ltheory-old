#ifndef LTE_Mouse_h__
#define LTE_Mouse_h__

#include "Common.h"
#include "DeclareFunction.h"
#include "Enum.h"
#include "V2.h"

namespace LTE {
  DeclareFunctionNoParams(Mouse_DoubleClicked, bool)
  LT_API bool Mouse_Down(MouseButton button);
  LT_API bool Mouse_Pressed(MouseButton button);
  LT_API bool Mouse_Released(MouseButton button);

  DeclareFunctionNoParams(Mouse_GetDP, V2)
  DeclareFunctionNoParams(Mouse_GetDX, int)
  DeclareFunctionNoParams(Mouse_GetDY, int)
  DeclareFunctionNoParams(Mouse_GetIdleTime, float)
  DeclareFunctionNoParams(Mouse_GetPos, V2)
  DeclareFunctionNoParams(Mouse_GetPosImmediate, V2)
  DeclareFunctionNoParams(Mouse_GetPosLast, V2)
  DeclareFunctionNoParams(Mouse_GetX, int)
  DeclareFunctionNoParams(Mouse_GetY, int)

  LT_API float Mouse_GetDownTime(MouseButton button);
  DeclareFunctionNoParams(Mouse_GetScrollDelta, float)

  LT_API void Mouse_SetPos(V2 const& v);
  LT_API void Mouse_SetPressed(MouseButton button, bool pressed);
  LT_API void Mouse_SetScrollDelta(float ds);

  LT_API void Mouse_Update();
  LT_API void Mouse_UpdatePos(V2I const& p);

  DeclareFunctionNoParams(Mouse_LeftDown, bool)
  DeclareFunctionNoParams(Mouse_LeftPressed, bool)
  DeclareFunctionNoParams(Mouse_LeftReleased, bool)
  DeclareFunctionNoParams(Mouse_RightDown, bool)
  DeclareFunctionNoParams(Mouse_RightPressed, bool)
  DeclareFunctionNoParams(Mouse_RightReleased, bool)
}

#endif

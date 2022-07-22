#include "ClipRegion.h"
#include "WidgetRenderer.h"

#include "LTE/Renderer.h"
#include "LTE/Vector.h"
#include "LTE/Viewport.h"

namespace {
  AutoClass(ClipRegion,
    V2, lower,
    V2, upper)
    ClipRegion() {}
  };

  Vector<ClipRegion>& GetStack() {
    static Vector<ClipRegion> stack;
    return stack;
  }
}

DefineFunction(ClipRegion_GetMin) {
  return GetStack().back().lower;
}

DefineFunction(ClipRegion_GetMax) {
  return GetStack().back().upper;
}

DefineFunction(ClipRegion_Pop) {
  WidgetRenderer_Flush();
  Renderer_PopScissor();
  GetStack().pop();
}

DefineFunction(ClipRegion_Push) {
  WidgetRenderer_Flush();
  Viewport const& vp = Viewport_Get();
  V2 newPos = args.pos;
  V2 newSize = args.size;

  Vector<ClipRegion>& stack = GetStack();
  if (stack.size()) {
    V2 p1 = Max(stack.back().lower, newPos);
    V2 p2 = Min(stack.back().upper, newPos + newSize);
    newPos = p1;
    newSize = p2 - p1;
  }

  V2 pos(newPos.x, vp->size.y - (newPos.y + newSize.y));
  Renderer_PushScissorOn(pos * vp->resolution, newSize * vp->resolution);
  GetStack().push(ClipRegion(newPos, newPos + newSize));
}

DefineFunction(ClipRegion_PushNoClip) {
  WidgetRenderer_Flush();
  Renderer_PushScissorOff();
  Viewport const& vp = Viewport_Get();
  GetStack().push(ClipRegion(vp->position, vp->position + vp->size));
}

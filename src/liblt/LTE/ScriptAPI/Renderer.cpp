#include "LTE/Function.h"
#include "LTE/Renderer.h"

FreeFunctionNoParams(int, Renderer_GetDrawCallCount,
  "Return the number of renderer draw calls dispatched this frame")
{
  return Renderer_GetDrawCallCount();
}

FreeFunctionNoParams(int, Renderer_GetPolyCount,
  "Return the number of polygons drawn this frame")
{
  return Renderer_GetPolyCount();
}

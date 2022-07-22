#include "LTE/Renderable.h"
#include "LTE/Function.h"

FreeFunction(Bound3, Renderable_GetBound,
  "Return the local bounding box of 'renderable'",
  Renderable, renderable)
{
  return renderable->GetBound();
} FunctionAlias(Renderable_GetBound, GetBound);

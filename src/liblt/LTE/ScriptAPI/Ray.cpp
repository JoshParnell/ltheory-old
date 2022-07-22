#include "LTE/Ray.h"
#include "LTE/Function.h"

TypeAlias(RayD, Ray);

FreeFunction(V3D, Ray_GetPoint,
  "Return the point at 't' units along 'ray'",
  RayD, ray,
  double, t)
{
  return ray(t);
} FunctionAlias(Ray_GetPoint, GetPoint);

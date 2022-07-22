#include "Orientation.h"
#include "Game/Object.h"
#include "LTE/Function.h"
#include "LTE/Math.h"
#include "LTE/Matrix.h"

void ComponentOrientation::Rotate(V3 const& rotation) {
  RotateBasis(transform.right, transform.up, transform.look, rotation);
  version++;
}

FreeFunction(V3, Object_GetLook,
  "Return the look direction of 'object'",
  Object, object)
{
  return object->GetLook();
} FunctionAlias(Object_GetLook, GetLook);

FreeFunction(Position, Object_GetPos,
  "Return the position of 'object'",
  Object, object)
{
  return object->GetPos();
} FunctionAlias(Object_GetPos, GetPos);

FreeFunction(V3, Object_GetRight,
  "Return the right direction of 'object'",
  Object, object)
{
  return object->GetRight();
} FunctionAlias(Object_GetRight, GetRight);

FreeFunction(V3, Object_GetScale,
  "Return the axial scaling of 'object'",
  Object, object)
{
  return object->GetScale();
} FunctionAlias(Object_GetScale, GetScale);

FreeFunction(Transform, Object_GetTransform,
  "Return the global transform of 'object'",
  Object, object)
{
  return object->GetTransform();
} FunctionAlias(Object_GetTransform, GetTransform);

FreeFunction(V3, Object_GetUp,
  "Return the up direction of 'object'",
  Object, object)
{
  return object->GetUp();
} FunctionAlias(Object_GetUp, GetUp);

VoidFreeFunction(Object_SetLook,
  "Orient 'object' to face towards 'look' direction",
  Object, object,
  V3, look)
{
  object->SetLook(look);
} FunctionAlias(Object_SetLook, SetLook);

VoidFreeFunction(Object_SetPos,
  "Move 'object' to 'position'",
  Object, object,
  Position, position)
{
  object->SetPos(position);
} FunctionAlias(Object_SetPos, SetPos);

VoidFreeFunction(Object_SetScale,
  "Set the axial scaling of 'object' to 'scale'",
  Object, object,
  V3, scale)
{
  object->SetScale(scale);
} FunctionAlias(Object_SetScale, SetScale);

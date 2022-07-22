#include "Nameable.h"
#include "Game/Object.h"
#include "LTE/Function.h"

FreeFunction(String, Object_GetName,
  "Return the name of 'object'",
  Object, object)
{
  return object->GetName();
} FunctionAlias(Object_GetName, GetName);

VoidFreeFunction(Object_SetName,
  "Set the name of 'object' to 'name'",
  Object, object,
  String, name)
{
  return object->SetName(name);
} FunctionAlias(Object_SetName, SetName);

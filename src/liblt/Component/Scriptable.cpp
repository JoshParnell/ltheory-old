#include "Scriptable.h"
#include "Game/Object.h"
#include "LTE/Function.h"

VoidFreeFunction(Object_AddScript,
  "Add 'script' to 'object' to be executed each frame during update",
  Object, object,
  Data, script)
{
  object->AddScript(script);
} FunctionAlias(Object_AddScript, AddScript);

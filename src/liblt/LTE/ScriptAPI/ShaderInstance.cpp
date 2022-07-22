#include "LTE/Function.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Texture2D.h"

TypeAlias(Reference<ShaderInstanceT>, ShaderInstance);

FreeFunction(ShaderInstance, ShaderInstance_Clone,
  "Return a new copy of 'instance'",
  ShaderInstance, instance)
{
  return instance->Clone();
} FunctionAlias(ShaderInstance_Clone, Clone);

VoidFreeFunction(ShaderInstance_SetFloat,
  "Set the float variable 'name' in 'instance' to 'value'",
  ShaderInstance, instance,
  String, name,
  float, value)
{
  instance->SetFloat(name, value);
} FunctionAlias(ShaderInstance_SetFloat, SetFloat);

VoidFreeFunction(ShaderInstance_SetInt,
  "Set the int variable 'name' in 'instance' to 'value'",
  ShaderInstance, instance,
  String, name,
  int, value)
{
  instance->SetInt(name, value);
} FunctionAlias(ShaderInstance_SetInt, SetInt);

VoidFreeFunction(ShaderInstance_SetTexture2D,
  "Set the texture2d variable 'name' in 'instance' to 'value'",
  ShaderInstance, instance,
  String, name,
  Texture2D, value)
{
  instance->SetTexture2D(name, value);
} FunctionAlias(ShaderInstance_SetTexture2D, SetTexture2D);

VoidFreeFunction(ShaderInstance_SetVec2,
  "Set the vec2 variable 'name' in 'instance' to 'value'",
  ShaderInstance, instance,
  String, name,
  V2, value)
{
  instance->SetFloat2(name, value);
} FunctionAlias(ShaderInstance_SetVec2, SetVec2);

VoidFreeFunction(ShaderInstance_SetVec3,
  "Set the vec3 variable 'name' in 'instance' to 'value'",
  ShaderInstance, instance,
  String, name,
  V3, value)
{
  instance->SetFloat3(name, value);
} FunctionAlias(ShaderInstance_SetVec3, SetVec3);

VoidFreeFunction(ShaderInstance_SetVec4,
  "Set the vec4 variable 'name' in 'instance' to 'value'",
  ShaderInstance, instance,
  String, name,
  V4, value)
{
  instance->SetFloat4(name, value);
} FunctionAlias(ShaderInstance_SetVec4, SetVec4);

namespace {
  FunctionAlias(ShaderInstance_SetFloat, Set);
  FunctionAlias(ShaderInstance_SetInt, Set);
  FunctionAlias(ShaderInstance_SetTexture2D, Set);
  FunctionAlias(ShaderInstance_SetVec2, Set);
  FunctionAlias(ShaderInstance_SetVec3, Set);
  FunctionAlias(ShaderInstance_SetVec4, Set);
}

#include "LTE/Function.h"
#include "LTE/Model.h"
#include "LTE/ShaderInstance.h"

TypeAlias(Reference<ModelT>, Model);

DefineConversion(model_to_renderable, Model, Renderable) {
  dest = (Renderable)src;
}

FreeFunctionNoParams(Model, Model_Create, "Create a new, empty model") {
  return Model_Create();
}

VoidFreeFunction(Model_Add,
  "Add 'geometry' to 'model' with shader 'shaderInstance'",
  Model, model,
  Geometry, geometry,
  ShaderInstance, shaderInstance)
{
  model->Add(geometry, shaderInstance);
} FunctionAlias(Model_Add, Add);

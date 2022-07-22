#include "../RenderPasses.h"

#include "LTE/Data.h"
#include "LTE/DrawState.h"
#include "LTE/Renderer.h"
#include "LTE/Script.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Texture2D.h"

namespace {
  AutoClassDerived(CustomFilter, RenderPassT,
    Data, instance,
    ScriptFunction, render)
    DERIVED_TYPE_EX(CustomFilter)

    CustomFilter() {}

    char const* GetName() const {
      return "Custom Filter";
    }

    void OnRender(DrawState* state) {
      Texture2D const& input = state->primary;
      Texture2D result;
      render->VoidCall(&result, instance, input);
      state->primary = result;
    }
  };
}

DefineFunction(RenderPass_CustomFilter) {
  Reference<CustomFilter> self = new CustomFilter;
  ScriptType type = args.data.type->GetAux().Convert<ScriptType>();
  self->instance = args.data;
  self->render = type->GetFunction("Render");
  return self;
}

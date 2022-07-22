#include "../RenderPasses.h"

#include "LTE/DrawState.h"
#include "LTE/Renderer.h"
#include "LTE/Texture2D.h"

namespace {
  struct Clear : public RenderPassT {
    V4 value;
    DERIVED_TYPE_EX(Clear)

    Clear() {}

    Clear(V4 const& value) :
      value(value)
      {}

    char const* GetName() const {
      return "Clear";
    }

    void OnRender(DrawState* state) {
      Renderer_ResetCounters();
      state->primary->Bind(0);
      Renderer_Clear(value);
      state->primary->Unbind();

      for (uint i = 0; i < 2; ++i) {
        state->smallColor[i]->Bind(0);
        Renderer_Clear();
        state->smallColor[i]->Unbind();
      }
    }
  };

  struct ClearDepth : public RenderPassT {
    DERIVED_TYPE_EX(ClearDepth)

    char const* GetName() const {
      return "Clear Depth";
    }

    void OnRender(DrawState* state) {
      state->primary->Bind(0);
      Renderer_ClearDepth();
      state->primary->Unbind();
    }
  };
}

DefineFunction(RenderPass_Clear) {
  return new Clear(args.value);
}

DefineFunction(RenderPass_ClearDepth) {
  return new ClearDepth;
}

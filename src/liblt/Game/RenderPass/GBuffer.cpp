#include "../RenderPasses.h"

#include "Game/Object.h"
#include "Game/Graphics/RenderStyles.h"

#include "LTE/DrawState.h"
#include "LTE/Renderer.h"
#include "LTE/RenderStyle.h"
#include "LTE/StackFrame.h"
#include "LTE/Texture2D.h"

namespace {
  struct GBuffer : public RenderPassT {
    RenderStyle style;
    DERIVED_TYPE_EX(GBuffer)

    GBuffer() :
      style(RenderStyle_Default(false))
      {}

    char const* GetName() const {
      return "GBuffer Pass";
    }

    void OnRender(DrawState* state) {
      RenderStyle_Push(style);
      state->color[0]->Bind(0);
      state->color[1]->Bind(1);

      for (size_t i = 0; i < state->visible.size(); ++i)
        ((ObjectT*)state->visible[i])->OnDraw(state);

      state->color[1]->Unbind();
      state->color[0]->Unbind();
      RenderStyle_Pop();
    }
  };
}

DefineFunction(RenderPass_GBuffer) {
  return new GBuffer;
}

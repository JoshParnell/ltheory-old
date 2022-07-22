#include "../RenderPasses.h"

#include "LTE/Data.h"
#include "LTE/DrawState.h"
#include "LTE/Renderer.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Texture2D.h"

namespace {
  struct MotionBlur : public RenderPassT {
    Shader shader;
    DERIVED_TYPE_EX(MotionBlur)

    MotionBlur() :
      shader(Shader_Create("identity.jsl", "post/motionblur.jsl"))
      {}

    char const* GetName() const {
      return "Motion Blur";
    }

    void OnRender(DrawState* state) {
      float speed = Length(DrawState_Get("camVelocity").Convert<V3>());
      if (speed < 100.0f)
        return;

      Renderer_SetShader(*shader);
      (*shader)
        ("texture", state->primary)
        ("strength", (1.0f - Exp(-speed / 2000.0f)));

      state->secondary->Bind(0);
      Renderer_DrawFSQ();
      state->secondary->Unbind();
      state->Flip();
    }
  };
}

DefineFunction(RenderPass_MotionBlur) {
  return new MotionBlur;
}

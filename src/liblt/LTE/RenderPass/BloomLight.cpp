#include "../RenderPasses.h"

#include "LTE/DrawState.h"
#include "LTE/Renderer.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Texture2D.h"

namespace {
  struct BloomLight : public RenderPassT {
    Shader shaderBlur;
    Shader shaderComposite;
    Shader shaderThresh;
    int radius;
    DERIVED_TYPE_EX(BloomLight)

    BloomLight() {}

    BloomLight(int radius) :
      shaderBlur(Shader_Create("identity.jsl", "post/blurlight.jsl")),
      shaderComposite(Shader_Create("identity.jsl", "post/bloom_composite.jsl")),
      shaderThresh(Shader_Create("identity.jsl", "post/glowthresh.jsl")),
      radius(radius)
      {}

    char const* GetName() const {
      return "Bloom Light";
    }

    void OnRender(DrawState* state) {
      uint dsFactor = state->primary->GetWidth() / state->smallColor[0]->GetWidth();

      DrawState_Link(shaderThresh);
      DrawState_Link(shaderBlur);
      DrawState_Link(shaderComposite);

      (*shaderThresh)
        ("texture", state->primary);

      state->smallColor[0]->Bind(0);
      Renderer_DrawFSQ();
      state->smallColor[0]->Unbind();

      (*shaderBlur)
        ("radius", (int)(radius / dsFactor))
        ("texture", state->smallColor[0])
        ("rcpFrame",
          V2(1.0f / (float)state->smallColor[0]->GetWidth(),
             1.0f / (float)state->smallColor[0]->GetHeight()));

      state->smallColor[1]->Bind(0);
      Renderer_DrawFSQ();
      state->smallColor[1]->Unbind();

      (*shaderComposite)
        ("texture1", state->primary)
        ("texture2", state->smallColor[1]);

      state->secondary->Bind(0);
      Renderer_DrawFSQ();
      state->secondary->Unbind();
      state->Flip();
    }
  };
}

DefineFunction(RenderPass_BloomLight) {
  return new BloomLight(args.radius);
}


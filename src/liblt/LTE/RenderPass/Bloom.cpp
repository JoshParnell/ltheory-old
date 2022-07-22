#include "../RenderPasses.h"

#include "LTE/DrawState.h"
#include "LTE/Renderer.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Texture2D.h"

namespace {
  struct Bloom : public RenderPassT {
    Shader shaderBlur;
    Shader shaderComposite;
    Shader shaderThresh;
    int radius;
    float variance;
    DERIVED_TYPE_EX(Bloom)

    Bloom() {}

    Bloom(
        int radius,
        float variance) :
      shaderBlur(Shader_Create("identity.jsl", "post/blur.jsl")),
      shaderComposite(Shader_Create("identity.jsl", "post/bloom_composite.jsl")),
      shaderThresh(Shader_Create("identity.jsl", "post/glowthresh.jsl")),
      radius(radius),
      variance(variance)
      {}

    char const* GetName() const {
      return "Bloom";
    }

    void OnRender(DrawState* state) {
      int dsFactor = state->primary->GetWidth() / state->smallColor[0]->GetWidth();

      DrawState_Link(shaderThresh);
      DrawState_Link(shaderBlur);
      DrawState_Link(shaderComposite);

      (*shaderThresh)
        ("texture", state->primary);

      state->smallColor[0]->Bind(0);
      Renderer_DrawFSQ();
      state->smallColor[0]->Unbind();

      (*shaderBlur)
        ("radius", radius / dsFactor)
        ("texture", state->smallColor[0])
        ("variance", variance / dsFactor)
        ("delta", V2(0, 1.0f / (float)state->smallColor[0]->GetHeight()));

      state->smallColor[1]->Bind(0);
      Renderer_DrawFSQ();
      state->smallColor[1]->Unbind();

      (*shaderBlur)
        ("texture", state->smallColor[1])
        ("delta", V2(1.0f / (float)state->smallColor[1]->GetWidth(), 0));

      state->smallColor[0]->Bind(0);
      Renderer_DrawFSQ();
      state->smallColor[0]->Unbind();

      (*shaderComposite)
        ("texture1", state->primary)
        ("texture2", state->smallColor[0]);

      state->secondary->Bind(0);
      Renderer_DrawFSQ();
      state->secondary->Unbind();
      state->Flip();
    }
  };
}

DefineFunction(RenderPass_Bloom) {
  return new Bloom(args.radius, args.variance);
}

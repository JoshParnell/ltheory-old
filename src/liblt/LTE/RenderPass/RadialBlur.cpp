#include "../RenderPasses.h"

#include "LTE/DrawState.h"
#include "LTE/Renderer.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Texture2D.h"

namespace {
  struct RadialBlur : public RenderPassT {
    Shader shader;
    V2 center;
    float radius;
    float strength;
    float falloff;
    DERIVED_TYPE_EX(RadialBlur)

    RadialBlur() {}

    RadialBlur(
        V2 const& center,
        float radius,
        float strength,
        float falloff) :
      shader(Shader_Create("identity.jsl", "post/radialblur.jsl")),
      center(center),
      radius(radius),
      strength(strength),
      falloff(falloff)
      {}

    char const* GetName() const {
      return "Radial Blur";
    }

    void OnRender(DrawState* state) {
      Renderer_SetShader(*shader);
      (*shader)
        ("texture", state->primary)
        ("center", center)
        ("radius", radius)
        ("strength", strength)
        ("falloff", falloff);

      state->secondary->Bind(0);
      Renderer_DrawFSQ();
      state->secondary->Unbind();
      state->Flip();
    }
  };
}

DefineFunction(RenderPass_RadialBlur) {
  return new RadialBlur(args.center, args.radius, args.strength, args.falloff);
}

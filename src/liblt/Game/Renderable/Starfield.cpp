#include "../Renderables.h"

#include "Game/ShadingModels.h"

#include "LTE/DrawState.h"
#include "LTE/Math.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/RNG.h"
#include "LTE/ShaderInstance.h"

const float kBrightness = 0.04f;
const float kColoration = 0.40f;

namespace {
  Renderable Generate(Renderable_Starfield_Args const& args) {
    RNG rng = RNG_MTG(args.seed);

    static Texture2D texture;
    
    if (!texture) {
      static Shader generate = Shader_Create("identity.jsl", "gen/starbg.jsl");
      texture = Texture_Create(64, 64, GL_TextureFormat::R16F);
      texture->SetMinFilter(GL_TextureFilterMip::Linear);
      Texture_Generate(texture, generate);
    }

    ShaderInstance starShader = ShaderInstance_Create("skybox.jsl", "starbg.jsl");

    (*starShader)
      (RenderStateSwitch_BlendModeAdditive)
      (RenderStateSwitch_ZWritableOff)
      ("texture", texture);
    DrawState_Link(starShader);

    Model model = Model_Create()
      ->Add(Mesh_Box(2, true)->ReverseWinding(), ShadingModel_Skybox());

    Mesh starfield = Mesh_Create();
    for (uint i = 0; i < args.starCount; ++i) {
      float distance = 60000;
      float radius = 1.0f / 50.0f;
      Color color; 
      color.x = 1 + kColoration * rng->GetExp();
      color.y = 1 + kColoration * rng->GetExp();
      color.z = 1 + kColoration * rng->GetExp();
      color = Color_White + kColoration * Color_FromWavelength(rng->GetFloat(380, 780));
      color *= kBrightness * Pow(rng->GetExp(), 2.5f);

      V3 p = rng->GetDirection();
      V3 e2, e3;
      OrthoBasis(p, e2, e3);
      p *= distance;
      e2 *= radius * distance;
      e3 *= radius * distance;
      starfield
        ->AddVertex(p - e2, color, -1,  0)
        ->AddVertex(p + e3, color,  0,  1)
        ->AddVertex(p + e2, color,  1,  0)
        ->AddVertex(p - e3, color,  0, -1)
        ->AddQuadR(0, 3, 2, 1);
    }

    model->Add(starfield, starShader);
    return model;
  }
}

DefineFunction(Renderable_Starfield) {
  return Generate(args);
}

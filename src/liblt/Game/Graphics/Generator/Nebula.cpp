#include "../Generators.h"

#include "LTE/CubeMap.h"
#include "LTE/RNG.h"
#include "LTE/ShaderInstance.h"
#include "LTE/StackFrame.h"

const bool kAlwaysReload = false;
const size_t res = 1024;

namespace {
  V4 GetQuat(RNG const& rng) {
    float angle = 0.5f * rng->GetAngle();
    V3 axis = rng->GetDirection();
    float ca = Cos(angle);
    float sa = Sin(angle);
    return V4(sa * axis.x, sa * axis.y, sa * axis.z, ca);
  }

  CubeMap NebulaImpl(Generator_Nebula_Args const& args) {
    SFRAME("Generate Nebula");
    CubeMap self = CubeMap_Create(res, GL_TextureFormat::RGBA32F);
    RNG rng = RNG_MTG(args.seed);

    /* Generate nebulae. */ {
      static Shader shader = Shader_Create("identity.jsl", "gen/nebula.jsl");
      V4 q1 = GetQuat(rng);
      V4 q2 = GetQuat(rng);
      (*shader)
        ("color1", args.color1)
        ("color2", args.color2)
        ("offset", args.offset)
        ("orientation1", q1)
        ("orientation2", q2)
        ("roughness", args.roughness)
        ("seed", args.seed)
        ("starDir", args.starDir);
      self->GenerateFromShader(shader);
    }

#if 0
    /* Normalize radiance. */ {
      CubeMap lowRes = Generator_Blur(self, 0.75f, 128, 512)();
      CubeMap result = CubeMap_Create(res, GL_TextureFormat::RGBA16F);

      static Shader shader = Shader_Create("identity.jsl", "cubemap/multiply.jsl");
      (*shader)
        ("texture1", self)
        ("texture2", lowRes);
      result->GenerateFromShader(shader);
      self = result;
    }
#endif

    /* TODO : Permuate. */ {
      // static Shader permuteShader = Shader_Create("identity.jsl", "cube_permute.jsl");
      // CubeMap e2 = CubeMap_CreatCubeMap_Create();
      // e2->Create(res, GL_TextureFormat::RGBA16F);
    }
    return self;
  }
}

DefineFunction(Generator_Nebula) {
  return Cached(Bind(FreeFn(NebulaImpl), Generator_Nebula_Args(args)));
}

#include "../Generators.h"

#include "Game/Object/Planet.h"

#include "LTE/CubeMap.h"
#include "LTE/Math.h"
#include "LTE/RNG.h"
#include "LTE/ShaderInstance.h"
#include "LTE/StackFrame.h"

const size_t kResolution = 2048;

namespace {
  CubeMap Generate(uint seed) {
    SFRAME("Generate Planet Surface");
    static Shader shader = Shader_Create("identity.jsl", "gen/planet.jsl");

    CubeMap self = CubeMap_Create(kResolution);
    RNG rng = RNG_MTG(seed);

    (*shader)
      ("coef", Normalize(Pow(rng->GetV4(0.05f, 1), V4(2))))
      ("freq", 4 + rng->GetGaussian())
      ("seed", rng->GetFloat())
      ("power", 1 + 0.5f * rng->GetExp());

    self->GenerateFromShader(shader);
    return self;
  }
}

Generic<CubeMap> Generator_PlanetSurface(uint seed) {
  return Cached(Bind(FreeFn(Generate), seed));
}

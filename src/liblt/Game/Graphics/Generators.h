#ifndef Generators_h__
#define Generators_h__

#include "Game/Common.h"
#include "LTE/Color.h"
#include "LTE/Common.h"
#include "LTE/DeclareFunction.h"
#include "LTE/Generic.h"
#include "LTE/V3.h"
#include "LTE/V4.h"

/* CubeMap. */
DeclareFunctionArgBind(Generator_Blur, Generic<CubeMap>,
  Generic<CubeMap>, source,
  float, radius,
  size_t, resolution,
  size_t, samples)

DeclareFunctionArgBind(Generator_IRMap, Generic<CubeMap>,
  Generic<CubeMap>, source,
  size_t, samples)

DeclareFunctionArgBind(Generator_Nebula, Generic<CubeMap>,
  float, roughness,
  float, seed,
  Color, color1,
  Color, color2,
  V3, starDir,
  V4, offset)

LT_API Generic<CubeMap> Generator_PlanetSkybox(Planet const* planet);

LT_API Generic<CubeMap> Generator_PlanetSurface(uint seed);

/* Texture2D. */
LT_API Generic<Texture2D> Generator_LoadTexture2D(Location const& src);

LT_API Generic<Texture2D> Generator_NormalMap(
    Generic<Texture2D> const& source,
    float normalStrength = 0.1f);

LT_API Generic<Texture2D> Generator_ShaderInstance(
    size_t width,
    size_t height,
    ShaderInstance const& shader);

#endif

#ifndef ShadingModels_h__
#define ShadingModels_h__

#include "LTE/Generic.h"

LT_API ShaderInstance ShadingModel_Debug();

LT_API ShaderInstance ShadingModel_FlatColor(Color const& color);

LT_API ShaderInstance ShadingModel_Fresnel(
  Generic<Texture2D> const& albedoMap,
  Generic<Texture2D> const& normalMap);

LT_API ShaderInstance ShadingModel_Lambert(
  Generic<Texture2D> const& albedoMap,
  Generic<Texture2D> const& normalMap);

LT_API ShaderInstance ShadingModel_Metal(
  Generic<Texture2D> const& albedo,
  Generic<Texture2D> const& normal);

LT_API ShaderInstance ShadingModel_Skybox();

LT_API ShaderInstance ShadingModel_Water();

#endif

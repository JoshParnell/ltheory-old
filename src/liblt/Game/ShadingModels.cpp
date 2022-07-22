#include "ShadingModels.h"

#include "LTE/DrawState.h"
#include "LTE/Location.h"
#include "LTE/Shader.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Texture2D.h"

ShaderInstance ShadingModel_Debug() {
  static Shader shader;
  if (!shader)
    shader = Shader_Create("npm.jsl", "material/debug.jsl");

  ShaderInstance instance = ShaderInstance_Create(shader);
  DrawState_Link(instance);
  return instance;
}

ShaderInstance ShadingModel_FlatColor(Color const& color) {
  static Shader shader;
  if (!shader)
    shader = Shader_Create("skybox.jsl", "solidcolor.jsl");

  ShaderInstance instance = ShaderInstance_Create(shader);
  (*instance)
    ("color", color);
  DrawState_Link(instance);
  return instance;
}

ShaderInstance ShadingModel_Fresnel(
  Generic<Texture2D> const& albedoMap,
  Generic<Texture2D> const& normalMap)
{
  static Shader shader;
  if (!shader)
    shader = Shader_Create("npm.jsl", "material/fresnel.jsl");

  ShaderInstance instance = ShaderInstance_Create(shader);
  (*instance)
    ("albedoMap", albedoMap)
    ("normalMap", normalMap);
  DrawState_Link(instance);
  return instance;
}

ShaderInstance ShadingModel_Lambert(
  Generic<Texture2D> const& albedoMap,
  Generic<Texture2D> const& normalMap)
{
  static Shader shader;
  if (!shader)
    shader = Shader_Create("npm.jsl", "material/lambert.jsl");

  ShaderInstance instance = ShaderInstance_Create(shader);
  (*instance)
    ("albedoMap", albedoMap)
    ("normalMap", normalMap);
  DrawState_Link(instance);
  return instance;
}

ShaderInstance ShadingModel_Metal(
  Generic<Texture2D> const& albedo,
  Generic<Texture2D> const& normal)
{
  static Shader shader;
  static Texture2D detailMap;

  if (!shader) {
    shader = Shader_Create("npm.jsl", "material/metal.jsl");
    detailMap = Texture_Create(512, 512, GL_TextureFormat::RGBA8);
    Texture_Generate(detailMap, 
      Shader_Create("identity.jsl", "filter_rust.jsl"));
  }

  ShaderInstance instance = ShaderInstance_Create(shader);
  (*instance)
    ("albedoMap", albedo)
    ("normalMap", normal)
    ("detailMap", detailMap);
  DrawState_Link(instance);
  return instance;
}

ShaderInstance ShadingModel_Skybox() {
  static Shader shader;
  if (!shader)
    shader = Shader_Create("skybox.jsl", "skybox.jsl");

  ShaderInstance instance = ShaderInstance_Create(shader);
  (*instance)(RenderStateSwitch_ZWritableOff);
  DrawState_Link(instance);
  return instance;
}

ShaderInstance ShadingModel_Water() {
  static Shader shader;
  if (!shader)
    shader = Shader_Create("npm.jsl", "material/water.jsl");

  ShaderInstance instance = ShaderInstance_Create(shader);
  (*instance)
    (RenderStateSwitch_BlendModeAlpha)
    (RenderStateSwitch_ZWritableOff)
    /* CRITICAL */
    // ("atmoDensity", GlobalCallback::GetFogDensity)
    ("atmoScale", 0.01f)
    // ("atmoTint", GlobalCallback::GetFogColor)
    ("normalMap", Texture_LoadFrom(Location_Texture("water_normals.jpg")));
  DrawState_Link(instance);
  return instance;
}

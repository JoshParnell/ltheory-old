#include "../Generators.h"

#include "LTE/Shader.h"
#include "LTE/StackFrame.h"
#include "LTE/Texture2D.h"

namespace {
  Texture2D GeneratorNormalMap(
    Generic<Texture2D> const& source,
    float strength)
  {
    AUTO_FRAME;
    static Shader shader = Shader_Create("identity.jsl", "gen/normalmap_lum.jsl");

    Texture2D texture = source();
    Texture2D normalMap = Texture_Create(
      texture->GetWidth(),
      texture->GetHeight(),
      GL_TextureFormat::RGBA8);

    (*shader)
      ("cellW", 1.0f)
      ("cellH", 1.0f)
      ("height", strength)
      ("texture", texture)
      ("invWidth", 1.0f / (float)texture->GetWidth())
      ("invHeight", 1.0f / (float)texture->GetHeight());

    Texture_Generate(normalMap, shader);
    return normalMap;
  }
}

Generic<Texture2D> Generator_NormalMap(
  Generic<Texture2D> const& source,
  float strength)
{
  return Cached(Bind(FreeFn(GeneratorNormalMap), source, strength));
}

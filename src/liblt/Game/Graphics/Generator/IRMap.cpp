#include "../Generators.h"

#include "LTE/Array.h"
#include "LTE/CubeMap.h"
#include "LTE/RNG.h"
#include "LTE/ShaderInstance.h"
#include "LTE/StackFrame.h"
#include "LTE/Texture2D.h"
#include "LTE/Vector.h"

namespace {
  CubeMap Generate(Generator_IRMap_Args const& args) {
    SFRAME("Generate IRMap");
    static Shader shader = Shader_Create("identity.jsl", "cubemap/irmap.jsl");

    CubeMap const& source = args.source();
    size_t res = source->GetResolution();
    GL_TextureFormat::Enum format = source->GetFormat();

    CubeMap self = CubeMap_Create(res, format);
    size_t bpp = GL_TextureFormat::Size(format);

    /* Copy level 0, unchanged. */ {
      Array<uchar> buffer(Squared(source->GetResolution()) * bpp);
      for (uint i = 0; i < CubeFace::SIZE; ++i) {
        source->GetData((CubeFace::Enum)i, 0, buffer.data());
        self->SetData((CubeFace::Enum)i, 0, buffer.data());
      }
    }

    Texture2D sampleBuffer = Texture_Create(
      args.samples, 1, GL_TextureFormat::RGBA16F);

    /* Populate sample buffer. */ {
      RNG rng = RNG_MTG(128);
      Vector<V3> samples;
      for (uint i = 0; i < args.samples; ++i)
        samples.push(rng->GetExp() * rng->GetDirection());

      sampleBuffer->SetData(0, 0, args.samples, 1,
        GL_PixelFormat::RGB, GL_DataFormat::Float, samples.data());
    }

    uint levels = 0;
    for (uint i = res; i > 0; i /= 2)
      levels++;
    uint levelRes = res;

    for (uint i = 1; i < levels; ++i) {
      levelRes /= 2;
      float angle = (float)i / (float)(levels - 1);
      angle = Squared(angle);

      CubeMap thisMap = CubeMap_Create(levelRes, format);
      (*shader)
        ("angle", angle)
        ("source", source)
        ("samples", (int)args.samples)
        ("sampleBuffer", sampleBuffer);
      thisMap->GenerateFromShader(shader);

      /* Copy result to irMap mip level. */ {
        Array<float> buffer(Squared(levelRes) * bpp);
        for (uint j = 0; j < CubeFace::SIZE; ++j) {
          thisMap->GetData((CubeFace::Enum)j, 0, buffer.data());
          self->SetData((CubeFace::Enum)j, i, buffer.data());
        }
      }

      // self->SaveTo(Stringize() | "irmap_" | i, i);
    }

    return self;
  }
}

DefineFunction(Generator_IRMap) {
  return Cached(Bind(FreeFn(Generate), Generator_IRMap_Args(args)));
}

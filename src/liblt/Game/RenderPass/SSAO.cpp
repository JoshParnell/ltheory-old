#include "../RenderPasses.h"

#include "LTE/Array.h"
#include "LTE/DrawState.h"
#include "LTE/Math.h"
#include "LTE/Renderer.h"
#include "LTE/Shader.h"
#include "LTE/Texture2D.h"
#include "LTE/View.h"

const uint kSamples = 16;
const uint kNoiseBufferWidth = 1024;
const uint kNoiseBufferHeight = 768;

namespace {
  struct SSAO : public RenderPassT {
    Shader shader;
    Shader composite;
    Texture2D noiseBuffer;
    Array<V3> samples;
    DERIVED_TYPE_EX(SSAO)

    SSAO() :
      shader(Shader_Create("worldray.jsl", "post/ssao.jsl")),
      composite(Shader_Create("identity.jsl", "post/multiply.jsl"))
    {
      samples.resize(kSamples);
      for (uint i = 0; i < kSamples; ++i) {
        V3 sample = RandDirection();
        sample.z = Abs(sample.z);
        float r = Rand();
        samples[i] = r * sample;
      }

      Array<uchar> noiseValues(kNoiseBufferWidth * kNoiseBufferHeight * 4);
      uint i = 0;
      for (uint y = 0; y < kNoiseBufferHeight; ++y)
      for (uint x = 0; x < kNoiseBufferWidth; ++x)
      for (uint b = 0; b < 4; ++b)
        noiseValues[i++] = (uchar)(rand() % 256);

      noiseBuffer = Texture_Create(
        kNoiseBufferWidth,
        kNoiseBufferHeight,
        GL_TextureFormat::RGBA8, noiseValues.data());
    }

    char const* GetName() const {
      return "SSAO";
    }

    void OnRender(DrawState* state) {
      Texture2D const& ssaoBuffer = state->color[2];

      Matrix vp = Renderer_GetProjMatrix() * Renderer_GetViewMatrix();
      DrawState_Link(shader);
      (*shader)
        ("texture", state->color[0])
        ("normalBuffer", state->color[1])
        ("noiseBuffer", noiseBuffer)
        ("viewProj", &vp);
      shader->SetFloat3Array("kernel", samples.data(), samples.size());

      ssaoBuffer->Bind(0);
      Renderer_SetShader(*shader);
      Renderer_DrawFSQ();
      ssaoBuffer->Unbind();

      DrawState_Link(composite);
      (*composite)
        ("texture1", state->color[0])
        ("texture2", state->smallColor[0]);

      state->color[2]->Bind(0);
      Renderer_SetShader(*composite);
      Renderer_DrawFSQ();
      state->color[2]->Unbind();

      Swap(state->color[0], state->color[2]);
      state->primary = state->color[0];
      state->secondary = state->color[1];
      state->tertiary = state->color[2];
    }
  };
}

DefineFunction(RenderPass_SSAO) {
  return new SSAO;
}

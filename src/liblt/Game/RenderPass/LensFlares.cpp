#include "../RenderPasses.h"

#include "Game/Light.h"
#include "Game/Object.h"

#include "LTE/Array.h"
#include "LTE/DrawState.h"
#include "LTE/Location.h"
#include "LTE/Meshes.h"
#include "LTE/Renderer.h"
#include "LTE/Shader.h"
#include "LTE/Texture2D.h"
#include "LTE/Vector.h"
#include "LTE/View.h"

#include "Module/FrameTimer.h"

#include <algorithm>

const float kCullBrightness = 0.01f;
const float kOcclusionSpeed = 8.0f;
const uint kMaxFlares = 64;

namespace {
  AutoClass(LensFlare,
    V2, center,
    V2, scale,
    Color, color,
    float, depth,
    Light*, light)

    LensFlare() {}

    friend bool operator<(LensFlare const& a, LensFlare const& b) {
      return
        Luminance(a.color) * a.scale.GetMax() >
        Luminance(b.color) * b.scale.GetMax();
    }
  };

  struct LensFlares : public RenderPassT {
    Shader shader;
    Shader shaderComposite;
    Shader shaderComputeVisibility;
    Texture2D flareTexture;
    Texture2D dirtTexture;
    Texture2D queryBuffer;
    Texture2D resultBuffer;
    Array<V4> queryBufferData;
    Array<float> queryResultData;
    Vector<LensFlare> flares;
    DERIVED_TYPE_EX(LensFlares)

    LensFlares() :
      shader(Shader_Create("identity.jsl", "lensflare.jsl")),
      shaderComposite(Shader_Create("identity.jsl", "post/lensflare_composite.jsl")),
      shaderComputeVisibility(Shader_Create("identity.jsl", "compute/lensflare_visibility.jsl")),
      dirtTexture(Texture_LoadFrom(Location_Texture("lensdirt.jpg"))),
      queryBufferData(kMaxFlares),
      queryResultData(kMaxFlares)
    {
      queryBuffer = Texture_Create(kMaxFlares, 1, GL_TextureFormat::RGBA32F);
      resultBuffer = Texture_Create(kMaxFlares, 1, GL_TextureFormat::R32F);
    }

    char const* GetName() const {
      return "Lens Flares";
    }

    void OnRender(DrawState* state) {
      RendererZBuffer zBuffer(false);

      /* Generate flare texture. */ {
        static Shader generate = Shader_Create("identity.jsl", "gen/lensflare.jsl");
        if (!flareTexture) {
          flareTexture = Texture_Create(1024, 1024, GL_TextureFormat::R16F);
          Texture_Generate(flareTexture, generate);
        }
      }

      Texture2D const& targetBuffer = state->tertiary;

      /* Draw. */ {
        targetBuffer->Bind(0);
        DrawState_Link(shader);

        RendererBlendMode blendMode(BlendMode::Additive);
        Renderer_Clear();
        Renderer_SetShader(*shader);

        flares.clear();

        for (size_t i = 0; i < state->lights.size(); ++i) {
          Light* light = (Light*)state->lights[i];
          if (!light->parent || !light->flare)
            continue;
          if (light->color < kCullBrightness)
            continue;

          Transform const& transform = light->GetTransform();
          float r = 20.0f * light->radius * transform.scale.GetGeometricAverage();

          V3 projMin = state->view->Project(
            transform.pos - r * (state->view->transform.up + state->view->transform.right));
          V3 projMax = state->view->Project(
            transform.pos + r * (state->view->transform.up + state->view->transform.right));
          if (projMin.z < 0 || projMax.z < 0)
            continue;

          Position projected =
            state->view->proj.TransformPoint(
            state->view->view.TransformPoint(transform.pos));

          LensFlare flare(
            0.5f * (projMin + projMax).GetXY(),
            0.5f * (projMax - projMin).GetXY(),
            light->color,
            Min((float)projected.z, state->view->zFar),
            light);

          flares.push(flare);
        }

        std::sort(flares.begin(), flares.end());

        for (size_t i = 0; i < flares.size() && i < kMaxFlares; ++i) {
          LensFlare const& flare = flares[i];

          (*shader)
            ("baseColor", flare.color)
            ("center", flare.center)
            ("depth", flare.depth)
            ("opacity", flare.light->visibility)
            ("texture", flareTexture);

          Renderer_DrawQuad(
            flare.center - flare.scale,
            flare.center + flare.scale, -1, 1);
        }

        targetBuffer->Unbind();
      }

      /* Composite. */ {
        state->secondary->Bind(0);
        DrawState_Link(shaderComposite);
        (*shaderComposite)
          ("texture1", state->primary)
          ("texture2", targetBuffer)
          ("dirtTexture", dirtTexture);

        Renderer_SetShader(*shaderComposite);
        Renderer_DrawQuad();

        state->secondary->Unbind();
        state->Flip();
      }

      /* Compute Occlusion. */ {
        for (size_t i = 0; i < flares.size() && i < kMaxFlares; ++i)
          queryBufferData[i] =
            V4(flares[i].center.x, flares[i].center.y, flares[i].depth, 0);

        queryBuffer->SetData(
          0, 0,
          Min(kMaxFlares, (uint)flares.size()), 1,
          GL_PixelFormat::RGBA,
          GL_DataFormat::Float,
          queryBufferData.data());

        resultBuffer->Bind(0);

        (*shaderComputeVisibility)
          ("flareBuffer", queryBuffer);

        DrawState_Link(shaderComputeVisibility);
        Renderer_SetShader(*shaderComputeVisibility);
        Renderer_DrawQuad();

        resultBuffer->Unbind();
        resultBuffer->GetData(queryResultData.data());

        float factor = 1.0f - Exp(-kOcclusionSpeed * FrameTimer_Get());
        for (size_t i = 0; i < flares.size() && i < kMaxFlares; ++i)
          flares[i].light->visibility =
            Mix(flares[i].light->visibility, queryResultData[i], factor);
      }
    }
  };
}

DefineFunction(RenderPass_LensFlares) {
  return new LensFlares;
}

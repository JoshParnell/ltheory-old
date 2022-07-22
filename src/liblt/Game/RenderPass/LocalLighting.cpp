#include "../RenderPasses.h"

#include "Game/Light.h"
#include "Game/Object.h"

#include "LTE/DrawState.h"
#include "LTE/Meshes.h"
#include "LTE/Renderer.h"
#include "LTE/RenderStyle.h"
#include "LTE/Shader.h"
#include "LTE/Texture2D.h"
#include "LTE/View.h"

#include "Module/Settings.h"

const float kCullBrightness = 0.01f;
const uint kBatchSize = 16;
const uint kMaxLights = 16;

#include <algorithm>

namespace {
  AutoClass(LightData,
    V3, center,
    Color, color)
    LightData() {}
  };

  bool LightCompare(LightData const& a, LightData const& b) {
    return
      (Length(a.color) / Length(a.center)) >
      (Length(b.color) / Length(b.center));
  }

  struct LocalLighting : public RenderPassT {
    Shader shader;
    Shader shaderComposite;
    Vector<Color> colorsBuffer;
    Vector<V3> centersBuffer;
    Vector<LightData> lights;
    DERIVED_TYPE_EX(LocalLighting)

    LocalLighting() :
      shader(Shader_Create("worldray.jsl", "light/point.jsl")),
      shaderComposite(Shader_Create("worldray.jsl", "light/composite.jsl"))
      {}

    void Flush() {
      if (colorsBuffer.empty())
        return;

      while (colorsBuffer.size() < kBatchSize) {
        colorsBuffer.push(Color(0));
        centersBuffer.push(Color(0));
      }

      shader->SetFloat3Array("color", colorsBuffer.data(), colorsBuffer.size());
      shader->SetFloat3Array("center", centersBuffer.data(), colorsBuffer.size());
      Renderer_DrawQuad();

      colorsBuffer.clear();
      centersBuffer.clear();
    }

    char const* GetName() const {
      return "Local Lighting";
    }

    void OnRender(DrawState* state) {
      RendererZBuffer zBuffer(false);

      Texture2D const& lightBuffer = state->color[2];
      // Texture2D const& lightBuffer = state->smallColor[0];

      /* Local light accumulation. */
      FRAME("Accumulation") {
        lightBuffer->Bind(0);
        RendererBlendMode blendMode(BlendMode::Additive);

        DrawState_Link(shader);

        lights.clear();

        if (Settings_Bool("Graphics/Dynamic Lighting", true)()) {
          for (size_t i = 0; i < state->lights.size(); ++i) {
            Light* light = (Light*)state->lights[i];
            if (!light->parent)
              continue;
            if (light->color.GetSum() < kCullBrightness)
              continue;

            Transform const& transform = light->GetTransform();

            V3 offset = (V3)(transform.pos - state->view->transform.pos);
            if (Abs(offset).GetMax() < 0.1f)
              continue;

            lights.push(LightData(offset, Pow(light->color, V3(2.2f))));
          }
        }

        (*shader)
          ("normalBuffer", state->color[1]);
        Renderer_SetShader(*shader);

        std::sort(lights.begin(), lights.end(), LightCompare);

        for (uint i = 0; i < lights.size() && i < kMaxLights; i++) {
          centersBuffer.push(lights[i].center);
          colorsBuffer.push(lights[i].color);
          if (colorsBuffer.size() >= kBatchSize)
            Flush();
        }

        Flush();
        lightBuffer->Unbind();
      }

      FRAME("Compositing") {
        state->color[1]->Bind(0);
        DrawState_Link(shaderComposite);
        (*shaderComposite)
          ("albedoBuffer", state->color[0])
          ("lightBuffer", lightBuffer);

        Renderer_SetShader(*shaderComposite);
        Renderer_DrawQuad();

        state->color[1]->Unbind();
        state->primary = state->color[1];
        state->secondary = state->color[0];
        state->tertiary = state->color[2];
      }
    }
  };
}

DefineFunction(RenderPass_LocalLighting) {
  return new LocalLighting;
}

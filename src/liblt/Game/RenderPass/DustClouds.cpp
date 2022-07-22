#include "../RenderPasses.h"

#include "LTE/Array.h"
#include "LTE/Data.h"
#include "LTE/DrawState.h"
#include "LTE/Math.h"
#include "LTE/Renderer.h"
#include "LTE/Shader.h"
#include "LTE/StackArray.h"
#include "LTE/StackFrame.h"
#include "LTE/Texture2D.h"
#include "LTE/View.h"

const size_t kCloudCount = 4;
const float kBaseScale = 1024;
const float kScaleFactor = Pow(16.0f, (1.0f / (float)kCloudCount));

namespace {
  AutoClass(Cloud,
    Position, position,
    float, radius,
    float, alpha,
    V4, orientation)

    Cloud() {}
  };

  struct DustClouds : public RenderPassT {
    Shader shader;
    Array<Cloud> clouds;
    Array<V4> positions;
    Array<V4> orientations;
    Array<float> alphas;
    DERIVED_TYPE_EX(DustClouds)

    DustClouds() :
      shader(Shader_Create("worldray.jsl", "dustclouds.jsl")),
      clouds(kCloudCount),
      positions(kCloudCount),
      orientations(kCloudCount),
      alphas(kCloudCount)
    {
      float scale = kBaseScale;
      for (size_t i = 0; i < kCloudCount; ++i) {
        clouds[i].position = Position(0);
        clouds[i].radius = scale;
        clouds[i].alpha = Rand();
        clouds[i].orientation = GetQuat();
        scale *= kScaleFactor;
      }
    }

    char const* GetName() const {
      return "Dust Clouds";
    }

    V4 GetQuat() const {
      float angle = 0.5f * RandAngle();
      V3 axis = RandDirection();
      float ca = Cos(angle);
      float sa = Sin(angle);
      return V4(sa * axis.x, sa * axis.y, sa * axis.z, ca);
    }

    void OnRender(DrawState* state) {
      if (DrawState_Get("fogDensity").Convert<float>() < 0.01f)
        return;

      DrawState_Link(shader);
      (*shader)
        ("texture", state->primary);

      for (size_t i = 0; i < kCloudCount; ++i) {
        Distance d = Length(clouds[i].position - state->view->transform.pos);
        if (d > 0.5f * clouds[i].radius) {
          clouds[i].position = state->view->transform.pos;
          clouds[i].alpha = Rand();
          clouds[i].orientation = GetQuat();
        }

        positions[i] = V4((V3)(clouds[i].position - state->view->transform.pos),
                          clouds[i].radius);
        orientations[i] = clouds[i].orientation;
        alphas[i] = clouds[i].alpha;
      }

      shader->SetFloat4Array("positions", positions.data(), positions.size());
      shader->SetFloat4Array("orientations", orientations.data(), orientations.size());
      shader->SetFloatArray("alphas", alphas.data(), alphas.size());

      state->secondary->Bind(0);
      Renderer_PushBlendMode(BlendMode::Disabled);
      Renderer_PushZBuffer(false);
      Renderer_DrawQuad();
      Renderer_PopZBuffer();
      Renderer_PopBlendMode();
      state->secondary->Unbind();

      state->Flip();
    }
  };
}

DefineFunction(RenderPass_DustClouds) {
  return new DustClouds;
}

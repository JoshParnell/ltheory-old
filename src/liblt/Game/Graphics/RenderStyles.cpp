#include "RenderStyles.h"

#include "Module/Settings.h"

#include "LTE/DrawState.h"
#include "LTE/Geometry.h"
#include "LTE/Math.h"
#include "LTE/Matrix.h"
#include "LTE/Meshes.h"
#include "LTE/Pointer.h"
#include "LTE/Renderer.h"
#include "LTE/ShaderInstance.h"
#include "LTE/View.h"

namespace {
  struct RenderStyleDefaultImpl : public RenderStyleT {
    ShaderInstance currentShader;
    bool blended;
    bool willRender;

    RenderStyleDefaultImpl(bool blended) :
      blended(blended),
      willRender(true)
      {}

    void OnBegin() {
      Renderer_PushBlendMode(blended ? BlendMode::Alpha : BlendMode::Disabled);
      Renderer_PushZBuffer(true);
      Renderer_PushZWritable(!blended);
      willRender = true;
    }

    void OnEnd() {
      Renderer_PopBlendMode();
      Renderer_PopZBuffer();
      Renderer_PopZWritable();
    }

    void Render(Geometry const& geometry) {
      if (willRender) {
        currentShader->Begin();
        geometry->Draw();
        currentShader->End();
      }
    }

    void SetShader(ShaderInstanceT* shader) {
      bool hasBlending = shader->HasBlending();
      if ((hasBlending && blended) || (!hasBlending && !blended)) {
        willRender = true;
        if (currentShader != shader) {
          currentShader = shader;
          currentShader->GetShader()->SetInt("prepass", 0);
        }
        DrawState_Inject(currentShader->GetShader());
      } else {
        willRender = false;
      }
    }

    void SetTransform(Transform const& transform) {
      Renderer_SetWorldTransform(transform);
    }

    bool WillRender() const {
      return willRender;
    }
  };
}

RenderStyle RenderStyle_Default(bool blended) {
  return new RenderStyleDefaultImpl(blended);
}

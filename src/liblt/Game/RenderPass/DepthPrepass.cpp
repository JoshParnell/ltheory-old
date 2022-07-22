#include "../RenderPasses.h"

#include "Game/Object.h"

#include "LTE/DrawState.h"
#include "LTE/Geometry.h"
#include "LTE/Renderer.h"
#include "LTE/RenderStyle.h"
#include "LTE/Shader.h"
#include "LTE/ShaderInstance.h"
#include "LTE/StackFrame.h"
#include "LTE/Texture2D.h"
#include "LTE/View.h"

namespace {
  struct DepthPrepassStyle : public RenderStyleT {
    ShaderInstanceT* currentShader;
    Shader prepassShader;
    ShaderInstance state;
    bool willRender;

    DepthPrepassStyle() :
      currentShader(nullptr),
      prepassShader(Shader_Create("npm.jsl", "depthprepass.jsl")),
      willRender(true)
    {
      state = ShaderInstance_Create(prepassShader);
    }

    void OnBegin() {
      Renderer_PushBlendMode(BlendMode::Disabled);
      Renderer_PushZBuffer(true);
      Renderer_PushZWritable(true);
      willRender = true;
      currentShader = nullptr;
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
      if (currentShader == shader)
        return;

      currentShader = shader;
      willRender =
        !shader->HasBlending() &&
        !shader->HasState(RenderStateSwitch_ZWritableOff) &&
        !shader->HasState(RenderStateSwitch_ZBufferOff);

      if (willRender)
        currentShader->GetShader()->SetInt("prepass", 1);
    }

    void SetTransform(Transform const& transform) {
      Renderer_SetWorldTransform(transform);
    }

    bool WillRender() const {
      return willRender;
    }
  };

  struct DepthPrepass : public RenderPassT {
    Texture2D buffer;
    RenderStyle style;
    DERIVED_TYPE_EX(DepthPrepass)

    DepthPrepass() :
      style(new DepthPrepassStyle)
      {}

    char const* GetName() const {
      return "Depth Prepass";
    }

    void OnRender(DrawState* state) {
      if (!buffer ||
          buffer->GetWidth() != state->primary->GetWidth() ||
          buffer->GetHeight() != state->primary->GetHeight())
      {
        buffer = Texture_Create(
          state->primary->GetWidth(),
          state->primary->GetHeight(),
          GL_TextureFormat::R32F);
        buffer->SetMagFilter(GL_TextureFilter::Nearest);
        buffer->SetMinFilter(GL_TextureFilterMip::Nearest);
        buffer->SetWrapMode(GL_TextureWrapMode::ClampToEdge);
      }
      
      RenderStyle_Push(style);
      buffer->Bind(0);
      Renderer_Clear(V4(state->view->zFar));
      Renderer_ClearDepth();
      for (size_t i = 0; i < state->visible.size(); ++i)
        ((ObjectT*)state->visible[i])->OnDraw(state);
      Renderer_ClearDepth();

      buffer->Unbind();
      RenderStyle_Pop();
      state->depth = buffer.t;
    }
  };
}

DefineFunction(RenderPass_DepthPrepass) {
  return new DepthPrepass;
}

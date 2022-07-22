#include "../Widgets.h"

#include "LTE/DrawState.h"
#include "LTE/GL.h"
#include "LTE/Renderer.h"
#include "LTE/RenderPass.h"
#include "LTE/Shader.h"
#include "LTE/StackFrame.h"
#include "LTE/Texture2D.h"
#include "LTE/Transform.h"
#include "LTE/Vector.h"
#include "LTE/Viewport.h"

#include "UI/Widget.h"
#include "UI/WidgetRenderer.h"

const uint kDSFactor = 4;
const float kResolution = 1;

/* CRITICAL : Major memory leak upon resizing.
 * Cause?  ShaderInstances still bound to buffers (check refcount)? */

namespace {
  AutoClassDerived(WidgetRendered, WidgetComponentT,
    Vector<RenderPass>, passes)
    Shader presentShader;
    GL_Texture depthBuffer;
    Texture2D buffers[3];
    Texture2D smallBuffers[2];
    uint width;
    uint height;

    DERIVED_TYPE_EX(WidgetRendered)

    WidgetRendered() {}

    ~WidgetRendered() {
      Clear();
    }

    DefineInitializer {
      presentShader = Shader_Create("widgetTexture.jsl", "ui/texture.jsl");
      width = 0;
      height = 0;
      depthBuffer = GL_NullTexture;
    }

    void Clear() {
      if (depthBuffer != GL_NullTexture)
        GL_DeleteTexture(depthBuffer);
    }

    Texture2D CreateBuffer(
      uint width,
      uint height,
      GL_TextureFormat::Enum format)
    {
      Texture2D self = Texture_Create(width, height, format);
      self->SetMagFilter(GL_TextureFilter::Linear);
      self->SetMinFilter(GL_TextureFilterMip::Linear);
      self->SetWrapMode(GL_TextureWrapMode::ClampToEdge);
      return self;
    }

    void PreDraw(Widget const& self) {
      FRAME("Initialize Render") {
        uint targetWidth = (uint)(kResolution * self->size.x);
        uint targetHeight = (uint)(kResolution * self->size.y);

        if (!buffers[0] ||
             targetWidth != width ||
             targetHeight != height)
        {
          Clear();

          width = targetWidth;
          height = targetHeight;

          buffers[0] = CreateBuffer(width, height, GL_TextureFormat::RGBA16F);
          buffers[1] = CreateBuffer(width, height, GL_TextureFormat::RGBA16F);
          buffers[2] = CreateBuffer(width, height, GL_TextureFormat::RGBA16F);

          smallBuffers[0] = CreateBuffer(
            width / kDSFactor,
            height / kDSFactor,
            GL_TextureFormat::RGBA16F);

          smallBuffers[1] = CreateBuffer(
            width / kDSFactor,
            height / kDSFactor,
            GL_TextureFormat::RGBA16F);

          depthBuffer = GL_GenTexture();
          GL_BindTexture(GL_TextureTargetBindable::T2D, depthBuffer);
          GL_TexImage2D(
            GL_TextureTarget::T2D, 0,
            GL_TextureFormat::DepthComponent32F,
            width, height,
            GL_PixelFormat::DepthComponent,
            GL_DataFormat::Float, nullptr);
        }
      }

      Viewport vp = Viewport_Create(0, V2(width, height), 1, true);
      Viewport_Push(vp);
      DrawState state;
      state.Push();

      FRAME("Begin Render") {
        state.color[0] = buffers[0];
        state.color[1] = buffers[1];
        state.color[2] = buffers[2];
        state.smallColor[0] = smallBuffers[0];
        state.smallColor[1] = smallBuffers[1];
        state.primary = state.color[0];
        state.secondary = state.color[2];
        state.tertiary = state.color[1];

        Renderer_PushAllBuffers();
        Renderer_PushCullMode(CullMode::Backface);
        Renderer_PushDepthBuffer(depthBuffer);
        Renderer_PushScissorOff();
        Renderer_PushBlendMode(BlendMode::Disabled);
        Renderer_PushZBuffer(true);
      }

      for (size_t i = 0; i < passes.size(); ++i)
        passes[i]->Render(&state);

      FRAME("End Render") {
        Renderer_PopZBuffer();
        Renderer_PopBlendMode();
        Renderer_PopScissor();
        Renderer_PopDepthBuffer();
        Renderer_PopCullMode();
        Renderer_PopAllBuffers();
      }

      state.Pop();
      Viewport_Pop();

      FRAME("Present") {
        RendererState s(BlendMode::Alpha, CullMode::Disabled, false, false);
        DrawState_Link(presentShader);
        Renderer_SetShader((*presentShader)
          ("texture", state.primary)
          ("alpha", 1.0f));
        Renderer_DrawQuad(
          self->pos,
          self->pos + self->size,
          V2(0, 1),
          V2(1, 0));
        Renderer_Flush();
      }
    }

    void PostPosition(Widget const& self) {
      self->size = self->maxSize;
    }
  };
}

DefineFunction(Widget_Rendered) {
  return Widget_Create1(new WidgetRendered(args.passes));
} FunctionAlias(Widget_Rendered, Rendered);

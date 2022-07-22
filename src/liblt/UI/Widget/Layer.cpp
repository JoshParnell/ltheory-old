#include "../Widgets.h"

#include "LTE/Mesh.h"
#include "LTE/Mouse.h"
#include "LTE/Pool.h"
#include "LTE/Renderer.h"
#include "LTE/RenderPass.h"
#include "LTE/Shader.h"
#include "LTE/StackFrame.h"
#include "LTE/Texture2D.h"
#include "LTE/Transform.h"
#include "LTE/View.h"
#include "LTE/Viewport.h"
#include "LTE/Window.h"

#include "UI/Compositor.h"
#include "UI/Cursor.h"
#include "UI/Widget.h"
#include "UI/WidgetRenderer.h"

#include "LTE/Debug.h"

namespace {
  AutoClassDerived(WidgetLayer, WidgetComponentT,
    Compositor, compositor,
    Mesh, surface,
    float, resolution)
    Texture2D buffer;
    Matrix projMatrix;

    DERIVED_TYPE_EX(WidgetLayer)
    POOLED_TYPE

    WidgetLayer() {}

    V2 Project(Widget const& self, V2 const& pos) {
      V2 ndc = Clamp(2.0f * (pos / self->size) - V2(1), V2(-1), V2(1));
      V2 uv;
      Ray r(V3(ndc, -1.0f), V3(0, 0, 1));
      return surface->Intersects(r, 0, 0, &uv)
        ? uv * self->size
        : V2(-100000);
    }

    void PreDraw(Widget const& self) {
      uint bufferWidth = (uint)(resolution * self->size.x);
      uint bufferHeight = (uint)(resolution * self->size.y);
      if (!buffer ||
           buffer->GetWidth() != bufferWidth ||
           buffer->GetHeight() != bufferHeight)
      {
        buffer = Texture_Create(
          bufferWidth,
          bufferHeight,
          GL_TextureFormat::RGBA16F);
        buffer->SetMinFilter(GL_TextureFilterMip::Linear);
        buffer->SetWrapMode(GL_TextureWrapMode::ClampToEdge);
      }

      Cursor_Push(Project(self, Cursor_Get()), Project(self, Cursor_GetLast()));
      Viewport_Push(Viewport_Create(0, self->size, V2(resolution), true));

      Renderer_PushZBuffer(false);
      Renderer_PushDepthBuffer(GL_NullTexture);
      Renderer_PushScissorOn(0, V2(bufferWidth, bufferHeight));
      buffer->Bind(0);
      Renderer_Clear();
      Renderer_SetWorldTransform(Transform_Identity());
      Renderer_SetViewTransform(Transform_Identity());
      projMatrix = Renderer_GetProjMatrix();
      Matrix newProj = Viewport_Get()->GetMatrix();
      Renderer_SetProjMatrix(newProj);
    }

    void PreUpdate(Widget const& self) {
      Cursor_Push(Project(self, Cursor_Get()), Project(self, Cursor_GetLast()));
    }

    void PostDraw(Widget const& self) {
      WidgetRenderer_Flush();
      buffer->Unbind();
      Viewport_Pop();
      Renderer_SetProjMatrix(projMatrix);
      Renderer_PopScissor();
      Renderer_PopDepthBuffer();
      Renderer_PopZBuffer();
      Cursor_Pop();
      compositor->Composite(buffer, surface);
    }

    void PostPosition(Widget const& self) {
      self->size = self->maxSize;
    }

    void PostUpdate(Widget const& self) {
      compositor->Update();
      Cursor_Pop();
    }
  };
}

DefineFunction(Widget_Layer) {
  args.widget->Add(new WidgetLayer(args.compositor, args.surface, args.resolution));
  return args.widget;
} FunctionAlias(Widget_Layer, Layer);

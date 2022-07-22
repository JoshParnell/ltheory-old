#include "UI/WidgetRenderer.h"

#include "LTE/Color.h"
#include "LTE/Data.h"
#include "LTE/DrawState.h"
#include "LTE/Geometry.h"
#include "LTE/Math.h"
#include "LTE/Renderable.h"
#include "LTE/Renderer.h"
#include "LTE/RenderStyle.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Transform.h"
#include "LTE/View.h"
#include "LTE/Viewport.h"

namespace {

  struct HoloStyle : public RenderStyleT {
    ShaderInstance currentShader;
    ShaderInstance holoShader;

    HoloStyle() :
      holoShader(ShaderInstance_Create("npm.jsl", "hologram.jsl"))
      {}

    Shader const& GetShader () const {
      return holoShader->GetShader();
    }

    void OnBegin() {
      Renderer_PushBlendMode(BlendMode::Additive);
      Renderer_PushZBuffer(true);
      Renderer_PushZWritable(true);
      (*holoShader)("seed", Rand());
    }

    void OnEnd() {
      Renderer_PopBlendMode();
      Renderer_PopZBuffer();
      Renderer_PopZWritable();
    }

    void Render(Geometry const& geometry) {
      currentShader->Begin();
      geometry->Draw();
      currentShader->End();
    }

    void SetShader(ShaderInstanceT* shader) {
      bool hasBlending = shader->HasBlending();
      if (hasBlending)
        currentShader = shader;
      else
        currentShader = holoShader;

      currentShader->GetShader()->SetInt("prepass", 0);
      DrawState_Inject(currentShader->GetShader());
    }

    void SetTransform(Transform const& transform) {
      Renderer_SetWorldTransform(transform);
    }

    bool WillRender() const {
      return true;
    }
  };

  Reference<HoloStyle> holoStyle;

  void Initialize() {
    static bool init = false;
    if (!init) {
      init = true;
      holoStyle = new HoloStyle;
    }
  }
}

DefineFunction(WidgetRenderer_DrawRenderable) {
  Initialize();
  WidgetRenderer_Flush();

  Viewport const& currentVp = Viewport_Get();
  Viewport vp = Viewport_Create(args.pos, args.size, 1, currentVp->windowSpace);
  Viewport_Push(vp);
  DrawState state;
  state.Push();

#if 0
  float angle = Radians(60.0f);
  float h = 15.0f;
  float d = 0.5f * h / tan(0.5f * angle);
#endif
  View view(
    Transform_LookUp(args.camPos, args.camLook, args.camUp),
    args.camFov,
    args.size.x / args.size.y,
    0.05f,
    1.0e6f);

  Renderer_PushScissorOff();
  Renderer_SetViewTransform(view.transform);
  Renderer_SetProjMatrix(view.proj);

  Shader const& shader = holoStyle->GetShader();
  (*shader)
    ("baseAlpha", args.alpha)
    ("baseColor", V3(args.color))
    ("time", args.time);

  RenderStyle_Push(holoStyle);

  holoStyle->OnBegin();
  holoStyle->SetTransform(args.transform);
  args.renderable->Render(&state);
  holoStyle->OnEnd();

  RenderStyle_Pop();

  state.Pop();
  Viewport_Pop();
  Renderer_PopScissor();
} FunctionAlias(WidgetRenderer_DrawRenderable, DrawRenderable);

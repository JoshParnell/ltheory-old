#include "../RenderPasses.h"

#include "Component/Interior.h"
#include "Component/Zoned.h"

#include "Game/Camera.h"

#include "Module/Settings.h"

#include "LTE/DrawState.h"
#include "LTE/Renderer.h"
#include "LTE/RenderPasses.h"
#include "LTE/View.h"
#include "LTE/Viewport.h"

namespace {
  struct CameraPass : public RenderPassT {
    Camera camera;
    Vector<RenderPass> passes;
    DERIVED_TYPE_EX(CameraPass)

    CameraPass() {}

    CameraPass(Camera const& camera) : camera(camera) {
      passes.push(RenderPass_Visibility());
      passes.push(RenderPass_DepthPrepass());
      passes.push(RenderPass_GBuffer());
      // passes.push(RenderPass_SSAO());
      passes.push(RenderPass_GlobalLighting());
      passes.push(RenderPass_LocalLighting());
      passes.push(RenderPass_Blended());
      // passes.push(RenderPass_DustClouds());
      passes.push(RenderPass_Particles());
      passes.push(RenderPass_LensFlares());
      passes.push(RenderPass_Bloom(128, 64));
      passes.push(RenderPass_Tonemap());
      // passes.push(RenderPass_MotionBlur());
      passes.push(RenderPass_PostFilter("post/colorgrade1D.jsl"));
    }

    char const* GetName() const {
      return "Camera Pass";
    }

    void OnRender(DrawState* state) {
      ObjectT* target = camera->GetTarget();
      if (target)
        target->GetContainer()->AddInterior(camera);

      ObjectT* container = camera->GetContainer();
      if (!container)
        return;

      View v = camera->GetView(Viewport_Get()->GetAspect());
      Renderer_SetViewTransform(v.transform);
      Renderer_SetProjMatrix(v.proj);

      /* TODO : Cleaner zone detection. */
      Object zone;
      Position pos = camera->GetPos();
      float fogDensity = 0;

      for (ObjectType type = 0; type < ObjectType_SIZE; ++type) {
        for (InteriorTypeIterator it = Object_GetInteriorObjects(container, type);
             it.HasMore(); it.Advance())
        {
          Object const& object = it.Get();
          if (!object->GetZoned())
            break;

          Pointer<ComponentZoned> zone = object->GetZoned();
          fogDensity += zone->GetContainment(object, pos) * zone->fogDensity;
        }
      }

      state->view.push(&v);
      Camera_Push(camera);

      DrawState_Push("fogDensity", fogDensity);
      DrawState_Push("camVelocity", camera->GetTarget()
        ? camera->GetTarget()->GetVelocity()
        : V3(0));

      container->BeginDrawInterior(state);
      for (size_t i = 0; i < passes.size(); ++i)
        passes[i]->Render(state);
      container->EndDrawInterior(state);

      DrawState_Pop("fogDensity");
      DrawState_Pop("camVelocity");

      Camera_Pop();
      state->view.pop();
    }
  };
}

DefineFunction(RenderPass_Camera) {
  return new CameraPass(args.camera);
}

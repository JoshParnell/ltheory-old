#include "../Objects.h"

#include "Game/Camera.h"

#include "LTE/DrawState.h"
#include "LTE/Model.h"
#include "LTE/Meshes.h"
#include "LTE/RenderStyle.h"
#include "LTE/ShaderInstance.h"
#include "LTE/View.h"

const uint kFleckCount = 1024;
const float kDistance = 1024;

typedef ObjectWrapper
  < ObjectWrapperTail<ObjectType_Effect>
  >
  DustFlecksBaseT;

AutoClassDerived(DustFlecks, DustFlecksBaseT,
  Vector<Object>, elements)

  DERIVED_TYPE_EX(DustFlecks)

  DustFlecks() {}

  void OnDraw(DrawState* state) {
    static ShaderInstance shader;
    static Mesh mesh;

    if (!shader) {
      shader = ShaderInstance_Create("billboard_axis_wrapped.jsl", "dustfleck.jsl");
      (*shader)
        (RenderStateSwitch_BlendModeAdditive)
        (RenderStateSwitch_ZWritableOff);
      DrawState_Link(shader);

      mesh = Mesh_Create();
      for (uint i = 0; i < kFleckCount; ++i)
        mesh->AddMesh(Mesh_Billboard()->Translate(SampleSphere() * kDistance));
    }

    V3 velocity = Camera_Get()->GetTarget()->GetVelocity();
    (*shader)
      ("axis", Normalize(velocity))
      ("size", V2(4.0f, 0.06f * Min(1000.0f, Length(velocity))));

    RenderStyle const& style = RenderStyle_Get();
    style->SetTransform(Transform_Identity());
    style->SetShader(shader);
    style->Render(mesh);
  }
};

DERIVED_IMPLEMENT(DustFlecks)

DefineFunction(Object_DustFlecks) {
  return new DustFlecks();
}

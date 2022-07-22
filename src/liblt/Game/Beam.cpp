#include "Beam.h"

#include "Component/Collidable.h"
#include "Component/Interior.h"
#include "Component/Motion.h"
#include "Component/Queryable.h"

#include "Game/Graphics/Effects.h"
#include "Game/Item/WeaponType.h"

#include "LTE/AutoPtr.h"
#include "LTE/DrawState.h"
#include "LTE/Loader.h"
#include "LTE/Matrix.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/Ray.h"
#include "LTE/Renderable.h"
#include "LTE/RenderStyle.h"
#include "LTE/ShaderInstance.h"

#include "Module/SoundEngine.h"

const size_t kUpdatePeriod = 2;

namespace {
  Mesh gMesh;
  ShaderInstance gShader;

  void OnLoad() {
    gMesh = Mesh_Billboard(-1, 1, 0, 1);
    gShader = ShaderInstance_Create("billboard_axis.jsl", "beam.jsl");
    (*gShader)
      (RenderStateSwitch_BlendModeAdditive)
      (RenderStateSwitch_ZWritableOff);
    DrawState_Link(gShader);
  } bool r = RegisterLoader(OnLoad);
}

AutoClassDerived(BeamImpl, Beam,
  Object, hitObject,
  float, length,
  float, age,
  float, targetLength,
  int, tick,
  bool, soundPlayed)

  DERIVED_TYPE_EX(BeamImpl)

  struct RenderComponent : public RenderableT {
    DERIVED_TYPE(RenderComponent, RenderableT)
    BeamImpl* self;

    RenderComponent(BeamImpl* self) :
      self(self)
      {}

    void Render(DrawState* state) const {
      if (!self->Damager.source)
        return;

      (*gShader)
        ("t", self->age)
        ("axis", Normalize(self->direction))
        ("size", V2(6.f * self->width, self->length))
        ("thinness", 10.f)
        ("baseColor", self->Damager.type->color);

      RenderStyle const& style = RenderStyle_Get();
      style->SetTransform(Transform_Translation(self->origin));
      style->SetShader(gShader);
      style->Render(gMesh);
    }
  };

  BeamImpl() :
    length(0),
    age(0),
    targetLength(0),
    tick(RandI(0, kUpdatePeriod)),
    soundPlayed(false)
  {
    Drawable.renderable = (Renderable)(new RenderComponent(this));
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    if (!Damager.source) {
      Delete();
      return;
    }

    age += state.dt;
    length = Mix(length, targetLength, 5 * state.dt);

    if (--tick <= 0) {
      WorldRay ray(origin, Normalize(direction + RandV3(-0.01f, 0.01f)));
      targetLength = 0;
      float t;
      hitObject = GetContainer()->QueryInterior(
        ray, t, Damager.type->range, nullptr, true, RaycastCanCollideBidirectional, this);

      targetLength = hitObject ? t : Damager.type->range;
      tick = (int)(Rand(0.9f, 1.1f) * kUpdatePeriod);
    }

    if (hitObject)
      Damager.Hit(
        this, hitObject, origin + direction * targetLength,
        state.dt);

    if (!soundPlayed) {
      soundPlayed = true;
      Sound_Play3D("weapon/beam1_loop.wav", this, 0, 1.f, 1, true);
    }
  }

  bool CanCollide(const ObjectT* o) const {
    if (Damager.source && o->GetRoot() == Damager.source->GetRoot())
      return false;
    return true;
  }
};

DERIVED_IMPLEMENT(BeamImpl)

Beam* Beam_Create() {
  return new BeamImpl;
}

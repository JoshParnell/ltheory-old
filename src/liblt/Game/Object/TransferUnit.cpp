#include "../Objects.h"

#include "Component/Attachable.h"
#include "Component/BoundingBox.h"
#include "Component/Cullable.h"
#include "Component/Drawable.h"
#include "Component/Mineable.h"
#include "Component/Orientation.h"
#include "Component/Queryable.h"
#include "Component/Pluggable.h"
#include "Component/Supertyped.h"

#include "Game/Actions.h"
#include "Game/Light.h"
#include "Game/Messages.h"
#include "Game/Particles.h"
#include "Game/Socket.h"

#include "Module/SoundEngine.h"

#include "LTE/Color.h"
#include "LTE/DrawState.h"
#include "LTE/Meshes.h"
#include "LTE/ParticleSystem.h"
#include "LTE/Pool.h"
#include "LTE/RenderStyle.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Smooth.h"

const float kWidthMult = 4;
const Color kDefaultColor = Color(0.3f, 0.7f, 1.0f);
const Color kStressColor = Color(1.0f, 0.3f, 0.1f);

namespace {
  Mesh gMesh;
  ShaderInstance gShader;
}

typedef ObjectWrapper
  < Component_Attachable
  < Component_BoundingBox
  < Component_Cullable
  < Component_Drawable
  < Component_Orientation
  < Component_Pluggable
  < Component_Supertyped
  < ObjectWrapperTail<ObjectType_TransferUnit>
  > > > > > > > >
  TransferUnitBaseT;

AutoClassDerived(TransferUnit, TransferUnitBaseT,
  float, age,
  Smooth<Position>, targetPos,
  Smooth<float>, frequency,
  Smooth<float>, active,
  Smooth<Color>, color,
  LightRef, backLight,
  LightRef, frontLight,
  bool, firing)
  Sound loop1;
  Sound loop2;

  DERIVED_TYPE_EX(TransferUnit)
  POOLED_TYPE

  struct RenderComponent : public RenderableT {
    DERIVED_TYPE(RenderComponent, RenderableT)
    Pointer<TransferUnit> self;

    RenderComponent(TransferUnit* self) :
      self(self)
      {}

    Bound3 GetBound() const {
      if (self->active < 0.001f)
        return Bound3(0);

      float r = Length(self->targetPos.value - self->GetPos());
      return Bound3(-r, r);
    }

    void Render(DrawState* state) const {
      if (self->active < 0.001f)
        return;
      
      if (!gMesh) {
        gMesh = Mesh_Billboard(-1, 1, 0, 1);
        gShader = ShaderInstance_Create("billboard_axis.jsl", "transferbeam.jsl");
        (*gShader)
          (RenderStateSwitch_BlendModeAdditive)
          (RenderStateSwitch_ZWritableOff);
        DrawState_Link(gShader);
      }

      V3 dir = self->targetPos - self->GetTransform().pos;
      (*gShader)
        ("axis", Normalize(dir))
        ("color", self->active.value * V4(self->color.value, 1.0f))
        ("size",
          V2(kWidthMult * Length(self->GetScale()),
             Min(Length(dir), self->Supertyped.type->GetRange())))
        ("t", self->age);

      RenderStyle const& style = RenderStyle_Get();
      style->SetTransform(Transform_Translation(self->GetTransform().pos));
      style->SetShader(gShader);
      style->Render(gMesh);
    }
  };

  TransferUnit() :
    age(0),
    targetPos(0),
    frequency(1),
    active(0),
    color(kDefaultColor),
    firing(false)
    {}

  bool CanCollide(ObjectT const* object) const {
    return object->GetRoot() != GetRoot();
  }

  void OnMessage(Data& m) {
    BaseType::OnMessage(m);
    if (m.type == Type_Get<MessageFire>())
      firing = true;

    else if (m.type == Type_Get<MessageTargetPosition>())
      targetPos.target = m.Convert<MessageTargetPosition>().position;
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    Pluggable.powerRequest = Supertyped.type->GetPowerDrain();
    active.target = firing ? Saturate(GetPowerFraction()) : 0.0f;
    frequency.target = 0.2f * RandExp();
    age += state.dt;

    active.Update(4.0f * state.dt);
    color.Update(4.0f * state.dt);
    frequency.Update(0.25f * state.dt);
    targetPos.value = targetPos.target;

    if (!backLight) {
      backLight = Light_Create(this);
      backLight->radius = 2.0f;
    }

    if (!frontLight) {
      frontLight = Light_Create(this);
      frontLight->radius = 1.0f;
    }

    if (!loop1) {
      loop1 = Sound_Play3D("transferunit/loop.ogg", this, 0, 0, Length(GetScale()), true);
      loop2 = Sound_Play3D("transferunit/loop.ogg", this, 0, 0, Length(GetScale()), true);
      loop1->SetCursor(Rand() * loop1->GetDuration());
      loop2->SetCursor(Rand() * loop2->GetDuration());
    }

    loop1->SetVolume(0.2f * active);
    loop2->SetVolume(0.2f * active);
    loop1->SetPitch(frequency);
    loop2->SetPitch(frequency);

    ObjectT* root = GetRoot();
    float stress = 1.0f - Exp(-root->GetUsedCapacity() / root->GetCapability().Storage);
    color.target = Mix(kDefaultColor, kStressColor, stress);

    frontLight->color = active * (1.0f + 0.2f * RandExp()) * color;

    if (active > 0.5f) {
      Position origin = GetPos();
      V3 direction = Normalize((V3)(targetPos.value - origin));
      WorldRay ray(origin, direction);

      float t;
      float range = Supertyped.type->GetRange();
      ObjectT* hitObject = GetContainer()->QueryInterior(
        ray, t, range, nullptr, true, RaycastCanCollideBidirectional, this);

      t = Min(t, range);
      backLight->Attachable.SetPos(GetTransform().InversePoint(ray(t)));

      if (hitObject) {
        backLight->color = 4.0f * active * (1.0f + 0.2f * RandExp()) * color;
        if (hitObject->GetMineable())
          Action_Mine(this, hitObject, ray(t))->Execute(state);
      } else {
        backLight->color = Color(0);
      }
    } else {
      backLight->color = Color(0);
    }

    firing = false;
  }
};

DERIVED_IMPLEMENT(TransferUnit)

DefineFunction(Object_TransferUnit) {
  Reference<TransferUnit> self = new TransferUnit();
  self->SetSupertype(args.type);
  self->Drawable.renderable = (Renderable)(new TransferUnit::RenderComponent(self));
  return self;
}

#include "../Objects.h"

#include "Component/Damager.h"
#include "Component/Drawable.h"
#include "Component/Interior.h"
#include "Component/Motion.h"
#include "Component/Orientation.h"

#include "Game/Light.h"
#include "Game/Item/WeaponType.h"

#include "Module/SoundEngine.h"

#include "LTE/DrawState.h"
#include "LTE/Math.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/Pointer.h"
#include "LTE/Pool.h"
#include "LTE/Ray.h"
#include "LTE/ShaderInstance.h"

typedef ObjectWrapper
  < Component_Damager
  < Component_Drawable
  < Component_Motion
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_Missile>
  > > > > >
  MissileBaseT;

AutoClassDerived(Missile, MissileBaseT,
  LightRef, light,
  Object, target,
  V3, targetOffset,
  Object, trail,
  V3, thrust,
  float, age)
  Sound sound;

  DERIVED_TYPE_EX(Missile)
  POOLED_TYPE

  Missile() {}

  Missile(Object_Missile_ArgRefs const& args) :
    target(args.target),
    targetOffset(args.targetOffset),
    thrust(args.thrust),
    age(0)
  {
    Motion.velocity = args.velocity;
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    if (!trail) {
      trail = Object_Trail(this, 100, Damager.type->color, 2);
      trail->Update(state);
    }

    if (!light)  {
      light = Light_Create(this);
      light->radius = 16;
      light->color = Damager.type->color;
    }

    if (!sound)
      sound = Sound_Play3D("weapon/missile1_loop.ogg", this, V3(0), 0.5f, 0.05f, true);

    age += state.dt;
    Motion.force += thrust;
    Motion.velocity += 2.0f * SampleSphere();

    if (target) {
      Position targetPos = target->GetTransform().TransformPoint(targetOffset);
      Position impactPos;
      float impactTime = ComputeImpact(
        GetPos(), targetPos, V3(0), target->GetVelocity(),
        Damager.type->speed, &impactPos);

      if (impactTime < 0)
        impactPos = targetPos;

      Motion.velocity = Damager.type->speed * 
        Mix(Normalize(Motion.velocity),
            Normalize((V3)(impactPos - GetPos())), 1.0 - Exp(-0.5f * state.dt));
    }

    /* CRITICAL */
    WorldRay r = WorldRay::FromPoints(GetPos(), GetPos());

    float t;
    ObjectT* hitObject = GetContainer()->QueryInterior(r, t, 1, nullptr, true);
    ObjectT* parent = Damager.source ? Damager.source->GetRoot() : this;

    if (hitObject && hitObject->GetRoot() != parent) {
      if (Damager.Hit(this, hitObject, r(t), state.dt)) {
        for (size_t i = 0; i < 5; ++i) {
          Object e = Object_Explosion(ExplosionType_Fire, 0, Rand(4.0f, 6.0f));
          e->SetPos(r(t) + RandDirection() * 10.0f);
          e->SetScale(Rand(5.0f, 20.0f));
          GetContainer()->AddInterior(e);
        }

        Delete();
        return;
      }
    }

    if (age > Damager.type->range / Damager.type->speed) {
      Delete();
      return;
    }
  }
};

DERIVED_IMPLEMENT(Missile)

DefineFunction(Object_Missile) {
  return new Missile(args);
}

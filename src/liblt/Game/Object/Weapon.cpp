#include "../Objects.h"

#include "Component/Attachable.h"
#include "Component/BoundingBox.h"
#include "Component/Collidable.h"
#include "Component/Cullable.h"
#include "Component/Drawable.h"
#include "Component/Explodable.h"
#include "Component/Integrity.h"
#include "Component/Orientation.h"
#include "Component/Pluggable.h"
#include "Component/Supertyped.h"

#include "Game/Beam.h"
#include "Game/Light.h"
#include "Game/Messages.h"
#include "Game/Socket.h"
#include "Game/Graphics/Effects.h"
#include "Game/Item/WeaponType.h"

#include "LTE/Math.h"
#include "LTE/Pool.h"

const float kMinAngle = -0.25f;
const float kTrackSpeed = 2.0f * kTau;
const bool kInfiniteAmmo = true;

typedef ObjectWrapper
  < Component_Attachable
  < Component_BoundingBox
  < Component_Collidable
  < Component_Cullable
  < Component_Drawable
  < Component_Explodable
  < Component_Integrity
  < Component_Orientation
  < Component_Pluggable
  < Component_Supertyped
  < ObjectWrapperTail<ObjectType_Weapon>
  > > > > > > > > > > >
  WeaponBaseT;

AutoClassDerived(Weapon, WeaponBaseT,
  Position, targetPos,
  Object, targetObject,
  Object, beam,
  V3, targetHeading,
  float, cooldown,
  float, magazineTime,
  Quantity, magazine,
  float, flash,
  LightRef, light,
  bool, isFiring)

  DERIVED_TYPE_EX(Weapon)
  POOLED_TYPE

  Weapon() :
    targetHeading(V3(0, 0, 1)),
    cooldown(0),
    magazineTime(0),
    magazine(0),
    flash(0),
    isFiring(false)
    {}

  void Aim() {
    if (!IsAlive())
      return;

    V3 heading = GetRelativeHeading(targetPos);
    if (!heading.IsFinite())
      return;

    float headingLength = Length(heading);
    if (headingLength > FLT_EPSILON)
      targetHeading = heading / headingLength;
  }

  bool CanCollide(ObjectT const* other) const {
    return other->GetDamager();
  }

  bool CanFire() const {
    return cooldown <= 0 && (kInfiniteAmmo || magazine > 0 || GetMaxUses() == 0);
  }

  void Fire() {
    if (!IsAlive())
      return;

    Aim();

    Position origin = Orientation.transform.TransformPoint(GetWeaponType()->offset);
    V3 heading = GetLook();
    V3 toTarget = Normalize(targetPos - origin);

    /* If we're not actually close to the desired heading, don't fire yet. */
    if (Dot(toTarget, heading) < 0.99f)
      return;

    while (CanFire()) {
      /* Note that we'll add the cooldown BEFORE raycasting, so that we don't
       * end up raycasting every transform. */
      cooldown += Rand(1.0f / 1.1f, 1.1f) / GetWeaponType()->rate;

      /* Check to make sure our ship isn't in the way.  If it is, don't fire! */
      Ray r(origin + parent->GetVelocity(), heading);
      float t;
      if (GetContainer()->QueryInterior(r, t, 100, nullptr, false) == parent) {
        if (beam) {
          beam->Delete();
          beam = nullptr;
        }
        return;
      }

      Object ammo = GetWeaponType()->Fire(this, origin, heading, targetObject);
      if (!ammo)
        return;

      flash = 1;
      if (magazine >= 0)
        magazine--;

      GetContainer()->AddInterior(ammo);
      if (beam) {
        // TODO : ...?
        UpdateBeam();
        UpdateBeam();
      }
    }
  }

  float GetCooldown() const {
    if (magazine == 0 && GetMaxUses() > 0)
      return magazineTime / GetWeaponType()->magazineTime;
    return Saturate(1.0f - cooldown * GetWeaponType()->rate);
  }

  Quantity GetMaxUses() const {
    return Supertyped.type->GetUses();
  }

  float GetRange() const {
    return Supertyped.type->GetRange();
  }

  V3 GetRelativeHeading(Position const& target) const {
    Position origin = Orientation.transform.TransformPoint(GetWeaponType()->offset);
    V3 toTarget = Normalize(target - origin);
    return parent->GetTransform().InverseVector(toTarget);
  }
  
  WeaponType* GetWeaponType() const {
    return (WeaponType*)(ItemT*)Supertyped.type;
  }

  Quantity GetUses() const {
    return magazine;
  }

  void HandleHeading(float dt) {
    Socket const& sock = Pluggable.GetSocket(this);

    V3& look = Attachable.transform.look;
    float angle = Angle(look, targetHeading);

    if (angle > FLT_EPSILON) {
      float delta = Min(angle, dt * kTrackSpeed);
      look = Normalize(Mix(look, targetHeading, Saturate(delta / angle)));

      /* Confine the turret to the socket's upper hemisphere. */
      float dotNormal = Dot(look, sock.transform.up);
      if (dotNormal < kMinAngle)
        look = Normalize(look - (dotNormal - kMinAngle) * sock.transform.up);

      Pluggable.UpdateJoint(this);
    }
  }

  void OnMessage(Data& m) {
    BaseType::OnMessage(m);
    if (m.type == Type_Get<MessageFire>())
      isFiring = true;

    else if (m.type == Type_Get<MessageReload>())
      Reload();

    else if (m.type == Type_Get<MessageTargetPosition>())
      targetPos = m.Convert<MessageTargetPosition>().position;

    else if (m.type == Type_Get<MessageTargetObject>())
      targetObject = m.Convert<MessageTargetObject>().object;
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    WeaponType const* wType = GetWeaponType();

    if (!light) {
      light = Light_Create(this);
      light->Attachable.transform = Transform_Translation(wType->GetOffset());
      light->radius = 2.0f;
    }

    if (isFiring)
      Fire();

    cooldown -= state.dt;
    cooldown = Max(-state.dt, cooldown);
    flash = Mix(flash, 0.0f, 1.0f - Exp(-20.0f * state.dt));
    light->color = 4.0f * flash * Supertyped.type->GetColor();

    /* Magazine reloading. */
    if (magazine <= 0 && GetMaxUses() > 0) {
      magazineTime += state.dt * Rand(0.9f, 1.1f);
      if (magazineTime >= wType->magazineTime) {
        magazineTime = 0;
        // ObjectT* root = GetRoot();
        // magazine = Min(GetMaxUses(), root->GetItemCount(m));
        magazine = GetMaxUses();
        // if (magazine)
        //   root->AddItem(m, -magazine);
      }
    }

    /* Beam handling. */
    if (!isFiring && beam) {
      beam->Delete();
      beam = nullptr;
    } else if (beam) {
      UpdateBeam();
      flash = 1.0f;
    }

    HandleHeading(state.dt);

    isFiring = false;
    Aim();
  }

  void Reload() {
    if (magazine > 0 && magazine < GetMaxUses()) {
      // GetRoot()->AddItem(GetAmmoType(), magazine, true);
      magazine = 0;
    }
  }

  void UpdateBeam() {
    Beam* b = (Beam*)(ObjectT*)beam;
    b->direction = GetLook();
    b->origin = Orientation.transform.TransformPoint(GetWeaponType()->offset);
  }
};

DERIVED_IMPLEMENT(Weapon)

DefineFunction(Object_Weapon) {
  Reference<Weapon> self = new Weapon;
  self->SetSupertype(args.type);
  self->GetAttachable()->SetScale(args.type->GetScale());
  return self;
}

#include "../Objects.h"

#include "Component/Attachable.h"
#include "Component/BoundingBox.h"
#include "Component/Collidable.h"
#include "Component/Cullable.h"
#include "Component/Drawable.h"
#include "Component/Explodable.h"
#include "Component/Orientation.h"
#include "Component/Pluggable.h"
#include "Component/Sockets.h"
#include "Component/Supertyped.h"

#include "Game/Socket.h"
#include "Game/Light.h"
#include "Game/Messages.h"

#include "LTE/Math.h"
#include "LTE/Matrix.h"
#include "LTE/Pool.h"

const float kTrackSpeed = 2.0f * kTau;

typedef ObjectWrapper
  < Component_Attachable
  < Component_BoundingBox
  < Component_Collidable
  < Component_Cullable
  < Component_Drawable
  < Component_Orientation
  < Component_Pluggable
  < Component_Sockets
  < Component_Supertyped
  < ObjectWrapperTail<ObjectType_Turret>
  > > > > > > > > > >
  TurretBaseT;

AutoClassDerived(Turret, TurretBaseT,
  Position, target)

  DERIVED_TYPE_EX(Turret)
  POOLED_TYPE

  Turret() {}

  bool CanCollide(ObjectT const* other) const {
    return other->GetDamager();
  }

  void OnMessage(Data& m) {
    BaseType::OnMessage(m);
    if (m.type == Type_Get<MessageTargetPosition>())
      target = m.Convert<MessageTargetPosition>().position;
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    V3& look = Attachable.transform.look;
    V3 toTarget = Normalize(target - GetPos());
    toTarget = parent->GetTransform().InverseVector(toTarget);
    float angle = Angle(look, toTarget);

    if (angle > FLT_EPSILON) {
      float delta = Min(angle, state.dt * kTrackSpeed);
      look = Normalize(Mix(look, toTarget, Saturate(delta / angle)));
      Pluggable.UpdateJoint(this);
    }
  }
};

DERIVED_IMPLEMENT(Turret)

DefineFunction(Object_Turret) {
  Reference<Turret> self = new Turret;
  self->SetSupertype(args.type);
  self->Attachable.transform.scale = 0.25f;
  return self;
}

#include "../Objects.h"

#include "Component/Affectable.h"
#include "Component/Asset.h"
#include "Component/BoundingBox.h"
#include "Component/Cargo.h"
#include "Component/Collidable.h"
#include "Component/Crew.h"
#include "Component/Cullable.h"
#include "Component/Database.h"
#include "Component/Detectable.h"
#include "Component/Drawable.h"
#include "Component/Explodable.h"
#include "Component/Integrity.h"
#include "Component/Motion.h"
#include "Component/MotionControl.h"
#include "Component/Nameable.h"
#include "Component/Orientation.h"
#include "Component/Pilotable.h"
#include "Component/Scriptable.h"
#include "Component/Sockets.h"
#include "Component/Supertyped.h"
#include "Component/Targets.h"
#include "Component/Tasks.h"

#include "LTE/Math.h"

const uint kTrailLength = 64;

typedef ObjectWrapper
  < Component_Affectable
  < Component_Asset
  < Component_BoundingBox
  < Component_Cargo
  < Component_Collidable
  < Component_Crew
  < Component_Cullable
  < Component_Database
  < Component_Detectable
  < Component_Drawable
  < Component_Explodable
  < Component_Integrity
  < Component_Motion
  < Component_MotionControl
  < Component_Nameable
  < Component_Orientation
  < Component_Pilotable
  < Component_Scriptable
  < Component_Sockets
  < Component_Supertyped
  < Component_Targets
  < Component_Tasks
  < ObjectWrapperTail<ObjectType_Ship>
  > > > > > > > > > > > > > > > > > > > > > > >
  ShipBaseT;

AutoClassDerived(Ship, ShipBaseT,
  Object, trail)
  DERIVED_TYPE_EX(Ship)
  POOLED_TYPE
  
  Ship() {}

  float GetCullDistanceMult() const {
    return 1.0f;
  }

  Signature GetSignature() const {
    return Signature(GetRadius(), 8, 0.1f, 4);
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

#if 0
    if (!trail) {
      trail = Object_Trail(this, kTrailLength, V3(0.5f), 0.1f * GetRadius());
      trail->SetLocalTransform(Transform_Translation(GetLocalBound().GetFrontPoint()));
      trail->Update(state);
    }
#endif

    Motion.mass = GetSupertype()->GetMass() + Cargo.currentMass;
  }
};

DERIVED_IMPLEMENT(Ship)

DefineFunction(Object_Ship) {
  Reference<Ship> self = new Ship;
  self->SetSupertype(args.type);
  ScriptFunction_Load("Object/Ship:Init")->VoidCall(0, DataRef((Object)self));
  return self;
}

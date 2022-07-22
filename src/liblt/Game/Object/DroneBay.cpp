#include "../Items.h"
#include "../Objects.h"

#include "Component/Attachable.h"
#include "Component/Orientation.h"
#include "Component/Pluggable.h"
#include "Component/Supertyped.h"

#include "Game/Constants.h"
#include "Game/Icons.h"
#include "Game/Items.h"
#include "Game/Materials.h"
#include "Game/Messages.h"
#include "Game/Objects.h"
#include "Game/Socket.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Mass.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Scale.h"
#include "Game/Attribute/Speed.h"
#include "Game/Attribute/Value.h"

#include "LTE/Math.h"
#include "LTE/Model.h"
#include "LTE/Pool.h"
#include "LTE/SDFMesh.h"

#include "Module/SoundEngine.h"

#include "UI/Glyphs.h"

const float kDroneBayCooldown = 0.25f;

typedef ObjectWrapper
  < Component_Attachable
  < Component_Orientation
  < Component_Pluggable
  < Component_Supertyped
  < ObjectWrapperTail<ObjectType_DroneBay>
  > > > > >
  DroneBayBaseT;

AutoClassDerived(DroneBay, DroneBayBaseT,
  Position, targetPos,
  float, cooldown)

  DERIVED_TYPE_EX(DroneBay)
  POOLED_TYPE

  DroneBay() :
    targetPos(0),
    cooldown(0)
    {}

  void OnMessage(Data& m) {
    BaseType::OnMessage(m);

    if (m.type == Type_Get<MessageLaunch>()) {
      if (cooldown <= 0) {
        Sound_Play3D("dronebay/launch.ogg", this, 0, 0.1f)
          ->SetPitch(Rand(0.75f, 1.25f));
        cooldown = kDroneBayCooldown;
        GetContainer()->AddInterior(Object_Payload(
          (ObjectT*)GetRoot(),
          Item_DroneProspectingType(0, 0),
          GetPos(),
          5000.0f * Normalize(targetPos - GetPos()),
          GetRoot()->GetVelocity() + 10.0f * GetUp()));
      }
    }

    else if (m.type == Type_Get<MessageTargetPosition>())
      targetPos = m.Convert<MessageTargetPosition>().position;
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    cooldown = Max(0.0f, cooldown - state.dt);
  }
};

DERIVED_IMPLEMENT(DroneBay)

typedef
    Attribute_Icon
  < Attribute_Mass
  < Attribute_Name
  < Attribute_Scale
  < Attribute_Speed
  < Attribute_Value
  < ItemWrapper<ItemType_DroneBayType>
  > > > > > >
  DroneBayTypeBase;

AutoClassDerivedEmpty(DroneBayType, DroneBayTypeBase)
  DERIVED_TYPE_EX(DroneBayType)

  SocketType GetSocketType() const {
    return SocketType_Interior;
  }

  Object Instantiate(ObjectT* parent) {
    Reference<DroneBay> self = new DroneBay;
    self->SetSupertype(this);
    return self;
  }
};

DERIVED_IMPLEMENT(DroneBayType)

DefineFunction(Item_DroneBayType) {
  Mass mass = Constant_ValueToMass(args.value, args.compactness);
  float speed = Constant_SpeedRatio(args.speed);

  Reference<DroneBayType> self = new DroneBayType;
  self->icon = Icon_Dock();
  self->mass = mass;
  self->name = "Drone Bay";
  self->scale = Constant_MassToScale(mass);
  self->speed = speed;
  self->value = args.value;
  return self;
}

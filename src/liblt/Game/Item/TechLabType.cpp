#include "../Items.h"

#include "Game/Constants.h"
#include "Game/Icons.h"
#include "Game/Objects.h"
#include "Game/Attribute/Capability.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Mass.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/PowerDrain.h"
#include "Game/Attribute/Scale.h"
#include "Game/Attribute/Value.h"

#include "UI/Glyphs.h"

typedef
    Attribute_Capability
  < Attribute_Icon
  < Attribute_Mass
  < Attribute_Name
  < Attribute_PowerDrain
  < Attribute_Scale
  < Attribute_Value
  < ItemWrapper<ItemType_TechLabType>
  > > > > > > >
  TechLabTypeBase;

AutoClassDerivedEmpty(TechLabType, TechLabTypeBase)
  DERIVED_TYPE_EX(TechLabType)

  SocketType GetSocketType() const {
    return SocketType_Interior;
  }

  Object Instantiate(ObjectT* parent) {
    return Object_TechLab(this);
  }
};

DERIVED_IMPLEMENT(TechLabType)

DefineFunction(Item_TechLabType) {
  Mass mass = Constant_ValueToMass(args.value, args.compactness);
  float powerDrain = Constant_ValueToPowerDrain(args.value, args.efficiency);
  float rate = Constant_ValueToOutput(args.value, args.rate);

  Reference<TechLabType> self = new TechLabType;
  self->capability = Capability_Research(rate);
  self->icon = Icon_Task_Research();
  self->mass = mass;
  self->name = "Tech Lab";
  self->powerDrain = powerDrain;
  self->scale = Constant_MassToScale(mass);
  self->value = args.value;
  return self;
}

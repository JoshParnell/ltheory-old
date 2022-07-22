#include "../Items.h"

#include "Game/Constants.h"
#include "Game/Objects.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Integrity.h"
#include "Game/Attribute/Mass.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/PowerDrain.h"
#include "Game/Attribute/Scale.h"
#include "Game/Attribute/Value.h"

#include "LTE/Math.h"
#include "LTE/RNG.h"

#include "UI/Glyphs.h"

const Icon kIcon = Icon_Create()
  ->Add(Glyph_Ring(0, 1, 1, 1, 0))
  ->Add(Glyph_Box(V2(-1, 0), V2(0.1f, 0.05f), 1, 1))
  ->Add(Glyph_Box(V2( 1, 0), V2(0.1f, 0.05f), 1, 1));

typedef
    Attribute_Icon
  < Attribute_Integrity
  < Attribute_Mass
  < Attribute_Name
  < Attribute_PowerDrain
  < Attribute_Scale
  < Attribute_Value
  < ItemWrapper<ItemType_ShieldType>
  > > > > > > >
  ShieldTypeBase;

AutoClassDerivedEmpty(ShieldType, ShieldTypeBase)
  DERIVED_TYPE_EX(ShieldType)

  ShieldType() {}

  SocketType GetSocketType() const {
    return SocketType_Generator;
  }

  Object Instantiate(ObjectT* parent) {
    return Object_Shield(this);
  }
};

DERIVED_IMPLEMENT(ShieldType)

DefineFunction(Item_ShieldType) {
  RNG rg = RNG_MTG(args.seed);
  
  Mass mass = Constant_ValueToMass(args.value, args.compactness);
  float powerDrain = Constant_ValueToPowerDrain(args.value, args.efficiency);
  Health integrity = Constant_ValueToIntegrity(args.value, args.integrity);

  Reference<ShieldType> self = new ShieldType;
  self->icon = kIcon;
  self->integrity = integrity;
  self->mass = mass;
  self->name = "Shield";
  self->powerDrain = powerDrain;
  self->scale = Constant_MassToScale(mass);
  self->value = args.value;
  return self;
}

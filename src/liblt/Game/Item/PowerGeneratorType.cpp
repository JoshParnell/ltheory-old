#include "../Items.h"

#include "Game/Constants.h"
#include "Game/Objects.h"
#include "Game/Attribute/Capability.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Value.h"

#include "UI/Glyphs.h"

typedef
    Attribute_Capability
  < Attribute_Icon
  < Attribute_Name
  < Attribute_Value
  < ItemWrapper<ItemType_PowerGeneratorType>
  > > > >
  PowerGeneratorTypeBase;

AutoClassDerivedEmpty(PowerGeneratorType, PowerGeneratorTypeBase)
  DERIVED_TYPE_EX(PowerGeneratorType)

  SocketType GetSocketType() const {
    return SocketType_Generator;
  }

  Object Instantiate(ObjectT* parent) {
    return Object_PowerGenerator(this);
  }
};

DERIVED_IMPLEMENT(PowerGeneratorType)

DefineFunction(Item_PowerGeneratorType) {
  static Icon generatorIcon = Icon_Create()
    ->Add(Glyph_Arc(V2(-0.50f,  0.25f), 0.1f, 0.1f, 1, 1, 0, 1))
    ->Add(Glyph_Arc(V2( 0.50f,  0.25f), 0.1f, 0.1f, 1, 1, 0, 1))
    ->Add(Glyph_Arc(V2( 0.00f, -0.50f), 0.1f, 0.1f, 1, 1, 0, 1));

  Reference<PowerGeneratorType> self = new PowerGeneratorType;
  self->capability = Capability_Power(Constant_ValueToOutput(args.value));
  self->icon = generatorIcon;
  self->name = "Power Generator";
  self->value = args.value;
  return self;
}

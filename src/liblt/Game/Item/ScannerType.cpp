#include "../Items.h"

#include "Game/Constants.h"
#include "Game/Objects.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Metatype.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/PowerDrain.h"
#include "Game/Attribute/Range.h"
#include "Game/Attribute/Value.h"

#include "LTE/Script.h"

#include "UI/Glyphs.h"

typedef
    Attribute_Icon
  < Attribute_Metatype
  < Attribute_Name
  < Attribute_PowerDrain
  < Attribute_Range
  < Attribute_Value
  < ItemWrapper<ItemType_ScannerType>
  > > > > > >
  ScannerTypeBase;

AutoClassDerivedEmpty(ScannerType, ScannerTypeBase)
  DERIVED_TYPE_EX(ScannerType)

  SocketType GetSocketType() const {
    return SocketType_Generator;
  }

  Object Instantiate(ObjectT* parent) {
    return Object_Scanner(this);
  }
};

DERIVED_IMPLEMENT(ScannerType)

DefineFunction(Item_ScannerType) {
  Reference<ScannerType> self = new ScannerType;
  ScriptFunction_Load("Icons:Scanner")->Call(self->icon);
  self->metatype = Item_ScannerType_Args(args);
  self->name = "Scanner";
  self->powerDrain = 1.0f;
  self->range = Constant_RangeRatio(args.range);
  self->value = args.value;
  return self;
}

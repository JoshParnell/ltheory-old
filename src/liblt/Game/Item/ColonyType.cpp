#include "../Items.h"

#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Task.h"
#include "Game/Attribute/Traits.h"

#include "LTE/Math.h"

#include "UI/Glyphs.h"

typedef
    Attribute_Icon
  < Attribute_Name
  < Attribute_Task
  < Attribute_Traits
  < ItemWrapper<ItemType_ColonyType>
  > > > >
  ColonyTypeBase;

AutoClassDerivedEmpty(ColonyType, ColonyTypeBase)
  DERIVED_TYPE_EX(ColonyType)
};

DERIVED_IMPLEMENT(ColonyType)

DefineFunction(Item_ColonyType) {
  Reference<ColonyType> self = new ColonyType;
  self->name = args.name;
  self->icon = args.icon;
  self->task = args.task;
  self->traits = args.traits;
  return self;
}

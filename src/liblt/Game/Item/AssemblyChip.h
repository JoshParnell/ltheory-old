#ifndef Item_AssemblyChip_h__
#define Item_AssemblyChip_h__

#include "Game/Items.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Value.h"
#include "LTE/AutoClass.h"
#include "LTE/Vector.h"

typedef
    Attribute_Icon
  < Attribute_Name
  < Attribute_Value
  < ItemWrapper<ItemType_AssemblyChip>
  > > >
  AssemblyChipBase;

AutoClassDerived(AssemblyChip, AssemblyChipBase,
  Item, blueprint,
  Item, item,
  Vector<ItemQuantity>, requirements,
  float, duration)

  DERIVED_TYPE_EX(AssemblyChip)

  AssemblyChip() :
    duration(0)
    {}
};

#endif

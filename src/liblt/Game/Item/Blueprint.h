#ifndef Item_Blueprint_h__
#define Item_Blueprint_h__

#include "Game/Items.h"
#include "Game/ItemWrapper.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Value.h"
#include "LTE/AutoClass.h"
#include "LTE/Data.h"
#include "LTE/Pointer.h"

typedef
    Attribute_Icon
  < Attribute_Name
  < Attribute_Value
  < ItemWrapper<ItemType_Blueprint>
  > > >
  BlueprintBase;

AutoClass(Modifier,
  String, attribute,
  float, multiplier)
  Modifier() {}
};

AutoClassDerived(Blueprint, BlueprintBase,
  Pointer<Blueprint>, parent,
  Data, metatype,
  Item, assemblyChip)

  DERIVED_TYPE_EX(Blueprint)

  Blueprint() {}
};

#endif

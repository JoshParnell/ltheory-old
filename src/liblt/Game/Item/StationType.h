#ifndef Item_StationType_h__
#define Item_StationType_h__

#include "Game/Items.h"
#include "Game/Attribute/Capability.h"
#include "Game/Attribute/Docks.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Integrity.h"
#include "Game/Attribute/Mass.h"
#include "Game/Attribute/Metatype.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Renderable.h"
#include "Game/Attribute/Scale.h"
#include "Game/Attribute/Sockets.h"
#include "Game/Attribute/Value.h"

typedef
    Attribute_Capability
  < Attribute_Docks
  < Attribute_Icon
  < Attribute_Integrity
  < Attribute_Mass
  < Attribute_Metatype
  < Attribute_Name
  < Attribute_Renderable
  < Attribute_Scale
  < Attribute_Sockets
  < Attribute_Value
  < ItemWrapper<ItemType_StationType>
  > > > > > > > > > > >
  StationTypeBase;

AutoClassDerivedEmpty(StationType, StationTypeBase)
  DERIVED_TYPE_EX(StationType)
  Renderable interiorModel;

  LT_API Object Instantiate(ObjectT* parent = 0);
};

#endif

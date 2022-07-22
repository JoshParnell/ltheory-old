#ifndef Item_WeaponType_h__
#define Item_WeaponType_h__

#include "../Items.h"

#include "Game/Attribute/Capability.h"
#include "Game/Attribute/Color.h"
#include "Game/Attribute/Damage.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Integrity.h"
#include "Game/Attribute/Mass.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Offset.h"
#include "Game/Attribute/PowerDrain.h"
#include "Game/Attribute/Properties.h"
#include "Game/Attribute/Range.h"
#include "Game/Attribute/Rate.h"
#include "Game/Attribute/Renderable.h"
#include "Game/Attribute/Scale.h"
#include "Game/Attribute/Sound.h"
#include "Game/Attribute/Speed.h"
#include "Game/Attribute/Uses.h"

#include "LTE/AutoClass.h"

/* As a convention, we will store the "base" part of the weapon (the mount,
   which only swivels, it does not pitch) in the model's 1st mesh, and the
   "barrel" part of the weapon (the part that has full motion) in the model's
   0th mesh. */

typedef
    Attribute_Capability
  < Attribute_Color
  < Attribute_Damage
  < Attribute_Icon
  < Attribute_Integrity
  < Attribute_Mass
  < Attribute_Name
  < Attribute_Offset
  < Attribute_PowerDrain
  < Attribute_Properties
  < Attribute_Range
  < Attribute_Rate
  < Attribute_Renderable
  < Attribute_Scale
  < Attribute_Sound
  < Attribute_Speed
  < Attribute_Uses
  < ItemWrapper<ItemType_WeaponType>
  > > > > > > > > > > > > > > > > >
  WeaponTypeBase;

AutoClassDerived(WeaponType, WeaponTypeBase,
  WeaponClass, type,
  float, magazineTime,
  float, spread)

  DERIVED_TYPE_EX(WeaponType)

  WeaponType() :
    type(WeaponClass_Pulse),
    magazineTime(0),
    spread(0)
    {}

  SocketType GetSocketType() const {
    return SocketType_Turret;
  }

  LT_API Object Fire(
    ObjectT* parent,
    Position const& origin,
    V3 const& heading,
    Object const& target);

  LT_API float GetDPS() const;

  LT_API Object Instantiate(ObjectT* parent);
};

#endif

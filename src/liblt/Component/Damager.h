#ifndef Component_Damager_h__
#define Component_Damager_h__

#include "Common.h"
#include "Game/Object.h"
#include "Game/Item/WeaponType.h"
#include "LTE/AutoClass.h"

AutoClass(ComponentDamager,
  Reference<WeaponType>, type,
  Object, source)

  ComponentDamager() {}

  LT_API bool Hit(
    ObjectT* self,
    Object const& other,
    Position const& position,
    float dt);
};

AutoComponent(Damager)
};

#endif

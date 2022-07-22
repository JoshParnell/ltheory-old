#ifndef Component_Integrity_h__
#define Component_Integrity_h__

#include "Common.h"
#include "Game/Item.h"
#include "LTE/AutoClass.h"
#include "LTE/Pointer.h"

AutoClass(ComponentIntegrity,
  Health, health,
  Health, maxHealth,
  Item, dataDamaged,
  Item, dataDestroyed)

  ComponentIntegrity() :
    health(0),
    maxHealth(0)
    {}

  LT_API Damage ApplyDamage(ObjectT*, Damage);
  LT_API ItemT* GetDataDamaged(ObjectT const*) const;
  LT_API ItemT* GetDataDestroyed(ObjectT const*) const;
};

AutoComponent(Integrity)
  void SetSupertype(Item const& type) {
    LTE_ASSERT(type->GetIntegrity() > 0);
    Integrity.health = Integrity.maxHealth = type->GetIntegrity();

    BaseT::SetSupertype(type);
  }

  ItemT* GetDataDamaged() const {
    return Integrity.GetDataDamaged(this);
  }

  ItemT* GetDataDestroyed() const {
    return Integrity.GetDataDestroyed(this);
  }

  Health GetHealth() const {
    return BaseT::GetHealth() + Integrity.health;
  }

  Health GetMaxHealth() const {
    return BaseT::GetMaxHealth() + Integrity.maxHealth;
  }
};

#endif

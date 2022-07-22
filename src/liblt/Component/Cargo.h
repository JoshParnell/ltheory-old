#ifndef Component_Cargo_h__
#define Component_Cargo_h__

#include "Common.h"
#include "Game/Capability.h"
#include "Game/Item.h"
#include "LTE/AutoClass.h"
#include "LTE/Map.h"

typedef Map<Item, Quantity> CargoMapT;
typedef CargoMapT::iterator CargoIter;
typedef CargoMapT::const_iterator CargoIterC;

AutoClass(ComponentCargo,
  CargoMapT, elements,
  Mass, currentMass,
  Mass, capacity)

  ComponentCargo() :
    currentMass(0),
    capacity(0)
    {}

  LT_API bool Add(Item const& item, Quantity count, bool force = false);
  
  Quantity GetCount(Item const& item) const {
    Quantity const* count = elements.get(item);
    return count ? *count : 0;
  }
};

AutoComponent(Cargo)
  void SetSupertype(Item const& type) {
    Cargo.capacity = type->GetCapability().Storage;

    BaseT::SetSupertype(type);
  }

  bool AddItem(Item const& item, Quantity count, bool force = false) {
    return item->GetStorageType() == StorageType_Cargo
      ? Cargo.Add(item, count, force)
      : BaseT::AddItem(item, count, force);
  }

  Quantity GetItemCount(Item const& item) const {
    return item->GetStorageType() == StorageType_Cargo
      ? Cargo.GetCount(item)
      : BaseT::GetItemCount(item);
  }
 
  Mass GetUsedCapacity() const {
    return Cargo.currentMass;
  }

  Quantity GetValue() const {
    Quantity total = BaseT::GetValue();
    for (CargoIterC it = Cargo.elements.begin();
         it != Cargo.elements.end(); ++it)
      total += it->first->GetValue() * it->second;
    return total;
  }
};

#endif

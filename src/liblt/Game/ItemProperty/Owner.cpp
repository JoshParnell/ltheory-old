#include "../ItemPropertys.h"

#include "Game/Player.h"

#include "LTE/Pool.h"

namespace {
  AutoClassDerivedEmpty(ItemPropertyOwner, ItemPropertyT)
    mutable Player returnValue;

    DERIVED_TYPE_EX(ItemPropertyOwner)
    POOLED_TYPE

    DataRef Evaluate(Item const& item) const {
      Object const& object = ItemProperty_Object->Get(item);
      if (!object) {
        returnValue = nullptr;
        return returnValue;
      }

      returnValue = object->GetOwner();
      return returnValue;
    }

    String GetName() const {
      return "Owning Faction";
    }
  };
}

ItemProperty ItemProperty_Owner() {
  return new ItemPropertyOwner;
}

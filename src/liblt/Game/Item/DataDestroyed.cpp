#include "../Items.h"

#include "Game/Object.h"
#include "Game/Attribute/Hash.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Object.h"
#include "Game/Attribute/Value.h"

#include "LTE/Hash.h"
#include "LTE/Pool.h"

namespace {
  typedef
      Attribute_Hash
    < Attribute_Name
    < Attribute_Object
    < Attribute_Value
    < ItemWrapper<ItemType_DataDestroyed>
    > > > >
    ItemDataDestroyedBase;

  AutoClassDerivedEmpty(ItemDataDestroyed, ItemDataDestroyedBase)
    DERIVED_TYPE_EX(ItemDataDestroyed)
    POOLED_TYPE

    ItemDataDestroyed() {}

    StorageType const& GetStorageType() const {
      static StorageType storage = StorageType_Database;
      return storage;
    }
  };
}

Item Item_Data_Destroyed(Object const& object) {
  Reference<ItemDataDestroyed> self = new ItemDataDestroyed;
  self->hash = Hasher() | self->GetTypeGUID() | object->GetHash();
  self->name = object->GetName() + " Destruction Log";
  self->object = object;
  self->value = object->GetValue();
  return self;
}

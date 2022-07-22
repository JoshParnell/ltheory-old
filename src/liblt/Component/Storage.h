#ifndef Component_Storage_h__
#define Component_Storage_h__

#include "Common.h"
#include "Game/Player.h"
#include "LTE/Map.h"

typedef Map<Object, Object> StorageMapT;

AutoClass(ComponentStorage,
  StorageMapT, entries)

  ComponentStorage() {}

  LT_API Object Get(Object const& owner);
};

AutoComponent(Storage)
  Object GetStorageLocker(Object const& owner) {
    return Storage.Get(owner);
  }
};

#endif

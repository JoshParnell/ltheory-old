#include "Storage.h"

#include "Component/Cargo.h"

#include "Game/Objects.h"

Object ComponentStorage::Get(Object const& owner) {
  Object& locker = entries[owner];
  if (!locker) {
    locker = Object_Pod(100000);
    locker->SetName(owner->GetName() + "'s Locker");
  }
  return locker;
}

FreeFunction(Object, Object_GetStorageLocker,
  "Return 'owner's storage locker at 'object'",
  Object, object,
  Object, owner)
{
  return object->GetStorage()
    ? object->GetStorageLocker(owner)
    : nullptr;
} FunctionAlias(Object_GetStorageLocker, GetStorageLocker);

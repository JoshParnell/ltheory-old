#include "Cargo.h"
#include "Game/Object.h"
#include "LTE/Function.h"

bool ComponentCargo::Add(Item const& item, Quantity count, bool force) {
  if (count == 0)
    return true;

  /* Check if we have enough room. */
  Mass requiredMass = item->GetMass() * (float)count;
  Mass freeMass = capacity - currentMass;
  if (!force &&
      requiredMass > 0 &&
      freeMass < requiredMass)
    return false;

  if ((elements[item] += count) == 0)
    elements.erase(item);

  /* NOTE : We need to be careful when using floats. Error will accumulate,
   *        so instead of using accumulation we recompute current mass after
   *        each operation. Keep an eye on performance. */
#if 0
   currentMass += requiredMass;
#else
  currentMass = 0;
  for (CargoIter it = elements.begin(); it != elements.end(); ++it)
    currentMass += it->first->GetMass() * (float)it->second;
#endif

  return true;
}

AutoClass(CargoIterator,
  CargoIter, iterator,
  Object, object)

  CargoIterator() {}
};

FreeFunction(CargoIterator, Object_GetCargo,
  "Return an iterator to the cargo contents of 'object'",
  Object, object)
{
  return CargoIterator(object->GetCargo()->elements.begin(), object);
} FunctionAlias(Object_GetCargo, GetCargo);

VoidFreeFunction(CargoIterator_Advance,
  "Advance 'iterator'",
  CargoIterator, iterator)
{
  ++((CargoIterator&)iterator).iterator;
} FunctionAlias(CargoIterator_Advance, Advance);

FreeFunction(bool, CargoIterator_HasMore,
  "Return whether 'iterator' has more elements",
  CargoIterator, iterator)
{
  return iterator.iterator != iterator.object->GetCargo()->elements.end();
} FunctionAlias(CargoIterator_HasMore, HasMore);

FreeFunction(Item, CargoIterator_Item,
  "Return the item in 'iterator'",
  CargoIterator, iterator)
{
  return iterator.iterator->first;
} FunctionAlias(CargoIterator_Item, GetItem);

FreeFunction(Quantity, CargoIterator_Quantity,
  "Return the quantity in 'iterator'",
  CargoIterator, iterator)
{
  return iterator.iterator->second;
} FunctionAlias(CargoIterator_Quantity, GetQuantity);

FreeFunction(bool, Object_AddItem,
  "Add 'quantity' of 'item' to 'object', return whether operation was successful",
  Object, object,
  Item, item,
  Quantity, quantity)
{
  return object->AddItem(item, quantity);
} FunctionAlias(Object_AddItem, AddItem);

FreeFunction(Quantity, Object_GetCapacity,
  "Return the total cargo capacity of 'object'",
  Object, object)
{
  return object->GetCapability().Storage;
} FunctionAlias(Object_GetCapacity, GetCapacity);

FreeFunction(Quantity, Object_GetItemCount,
  "Return the the number of 'item' in 'object's cargo",
  Object, object,
  Item, item)
{
  return object->GetItemCount(item);
} FunctionAlias(Object_GetItemCount, GetItemCount);

FreeFunction(Quantity, Object_GetUsedCapacity,
  "Return the capacity of 'object's cargo that is in use",
  Object, object)
{
  return object->GetUsedCapacity();
} FunctionAlias(Object_GetUsedCapacity, GetUsedCapacity);

FreeFunction(bool, Object_RemoveItem,
  "Remove 'quantity' of 'item' from 'object', return whether operation was successful",
  Object, object,
  Item, item,
  Quantity, quantity)
{
  return object->RemoveItem(item, quantity);
} FunctionAlias(Object_RemoveItem, RemoveItem);

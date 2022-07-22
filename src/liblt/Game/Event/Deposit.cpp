#include "../Events.h"

#include "Game/Player.h"

#include "LTE/AutoClass.h"
#include "LTE/Math.h"
#include "LTE/Transform.h"

DefineFunction(Event_Deposit) {
  Quantity quantity = Min(args.quantity, args.object->GetItemCount(args.item));
  args.object->RemoveItem(args.item, quantity);
  args.target->GetStorageLocker(args.object->GetOwner())->AddItem(args.item, quantity);
  return nullptr;
}

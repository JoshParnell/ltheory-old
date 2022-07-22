#include "../Events.h"

#include "Game/Items.h"
#include "Game/Player.h"
#include "LTE/Pool.h"

namespace {
  AutoClassDerived(EventDamage, EventT, Event_Damage_Args, args)
    DERIVED_TYPE_EX(EventDamage)
    POOLED_TYPE

    EventDamage() {}

    String ToString() const {
      return Stringize()
        | args.source->GetName() | " dealt  "
        | args.damage | " damage to "
        | args.dest->GetName();
    }
  };
}

DefineFunction(Event_Damage) {
  if (!args.dest->IsAlive())
    return nullptr;

  args.dest->ApplyDamage(args.damage);

  Pointer<ObjectT> sourceRoot = args.source->GetRoot();

  /* If the object has an owner, notify the owner that their asset is under
   * attack. */
  Player const& sourceOwner = sourceRoot->GetOwner();
  Player const& destOwner = args.dest->GetOwner();
  if (sourceOwner && destOwner)
    destOwner->OnAttacked(sourceOwner);

  /* Record a log of this damage. */
  sourceRoot->AddItem(Item_Data_Damaged(args.dest), args.damage);

  return new EventDamage(args);
}

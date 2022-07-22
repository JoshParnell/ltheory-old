#ifndef Game_Events_h__
#define Game_Events_h__

#include "Event.h"
#include "Item.h"
#include "Object.h"

#include "LTE/DeclareFunction.h"

DeclareFunctionArgBind(Event_Damage, Event,
  Object, source,
  Object, dest,
  Damage, damage)

DeclareFunctionArgBind(Event_Deposit, Event,
  Object, object,
  Object, target,
  Item, item,
  Quantity, quantity)

DeclareFunctionArgBind(Event_Destroyed, Event,
  Object, source,
  Object, dest)

DeclareFunctionArgBind(Event_Mined, Event,
  Object, object,
  Object, target,
  Item, item,
  Quantity, quantity)

#if 0
DeclareFunction(Event_Withdraw, Event,
  Object, object,
  Object, target,
  Item, item,
  Quantity, quantity)
#endif

#endif

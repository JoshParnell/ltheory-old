#ifndef Game_Order_h__
#define Game_Order_h__

#include "Common.h"

#include "LTE/AutoClass.h"
#include "LTE/DeclareFunction.h"
#include "LTE/Pool.h"
#include "LTE/Reference.h"

AutoClassDerived(OrderT, RefCounted,
  Object, owner,
  Item, item,
  Quantity, volume,
  Quantity, price,
  Quantity, filledVolume,
  Quantity, filledTotal,
  Object, node)
  POOLED_TYPE

  OrderT() :
    volume(0),
    price(0),
    filledVolume(0),
    filledTotal(0)
    {}
};

DeclareFunction(Order_Create, Order,
  Object, owner,
  Item, item,
  Quantity, volume,
  Quantity, price)

#endif

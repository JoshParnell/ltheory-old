#include "Order.h"

#include "Item.h"
#include "Object.h"

DefineFunction(Order_Create) {
  Order self = new OrderT;
  self->owner = args.owner;
  self->item = args.item;
  self->volume = args.volume;
  self->price = args.price;
  return self;
}

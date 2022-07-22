#include "Game/Item.h"
#include "Game/Object.h"
#include "Game/Task.h"

#include "LTE/Color.h"
#include "LTE/Data.h"
#include "LTE/Renderable.h"
#include "LTE/Function.h"

#include "UI/Icon.h"

TypeAlias(Reference<ItemT>, Item);

FreeFunction(bool, Item_Equal, "Return a == b", Item, a, Item, b) {
  return a == b;
} FunctionAlias(Item_Equal, ==);

FreeFunction(String, Item_GetTypeString,
  "Return the type of 'item'",
  Item, item)
{
  return ItemType_String[item->GetType()];
} FunctionAlias(Item_GetTypeString, GetTypeString);

FreeFunction(bool, Item_NotEqual, "Return a != b", Item, a, Item, b) {
  return a != b;
} FunctionAlias(Item_NotEqual, !=);

FreeFunction(Object, Item_Instantiate,
  "Return an object instantiation of 'item'",
  Item, item)
{
  return item->Instantiate();
} FunctionAlias(Item_Instantiate, Instantiate);

#define X(type, name, init)                                                    \
  FreeFunction(bool, Item_Has##name,                                           \
    "Return whether 'item' has the " #name " attribute",                       \
    Item, item)                                                                \
  {                                                                            \
    return item->Has##name();                                                  \
  } FunctionAlias(Item_Has##name, Has##name);                                  \
                                                                               \
  FreeFunction(type, Item_Get##name,                                           \
    "Return the " #name " of 'item'",                                          \
    Item, item)                                                                \
  {                                                                            \
    return item->Get##name();                                                  \
  } FunctionAlias(Item_Get##name, Get##name);                                  \
                                                                               \
  VoidFreeFunction(Item_Set##name,                                             \
    "Set the " #name " of 'item' to 'value'",                                  \
    Item, item,                                                                \
    type, value)                                                               \
  {                                                                            \
    if (item->Has##name())                                                     \
      (type&)item->Get##name() = value;                                        \
  } FunctionAlias(Item_Set##name, Set##name);
ITEMPROPERTY_X
#undef X

#define Z(x, y, z)                                                             \
  FreeFunction(bool, Item_Is##y,                                               \
  "Return whether 'item' is a " z,                                             \
  Item, item)                                                                  \
  {                                                                            \
    return item->GetType() == ItemType_##y;                                    \
  } FunctionAlias(Item_Is##y, Is##y);
ITEM_X
#undef Z

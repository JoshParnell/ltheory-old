#include "Item.h"

#include "AI/Traits.h"
#include "Game/Capability.h"
#include "Game/Object.h"
#include "Game/Task.h"
#include "LTE/Data.h"
#include "LTE/Renderable.h"
#include "LTE/Static.h"
#include "UI/Icon.h"

ItemT::ItemT() {
  Data& nextItemID = Static_Get("nextItemID");
  if (!nextItemID)
    nextItemID = ItemID(0);
  id = nextItemID.Convert<ItemID>()++;
}

Object ItemT::Instantiate(ObjectT* parent) {
  return nullptr;
}

bool ItemT::IsType(Item const& type) const {
  if (this == type.t)
    return true;
  ItemT* super = GetSuperType();
  return super ? super->IsType(type) : false;
}

#define X(type, name, default)                                                 \
  type const& ItemT::Get##name() const {                                       \
    static type value;                                                         \
    value = default;                                                           \
    return value;                                                              \
  }
ITEMPROPERTY_X
#undef X

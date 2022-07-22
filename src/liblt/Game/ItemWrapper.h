#ifndef ItemWrapper_h__
#define ItemWrapper_h__

#include "Game/Item.h"

template <int ItemTypeEnum>
ItemType const& Item_GetType() {
  static ItemType type = (ItemType)ItemTypeEnum;
  return type;
}

template <int ItemTypeEnum>
struct ItemWrapper : public ItemT {
  ItemType const& GetType() const {
    return Item_GetType<ItemTypeEnum>();
  }
};

#endif

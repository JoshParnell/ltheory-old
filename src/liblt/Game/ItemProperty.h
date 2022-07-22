#ifndef Game_ItemProperty_h__
#define Game_ItemProperty_h__

#include "Item.h"
#include "LTE/BaseType.h"
#include "LTE/Data.h"

struct ItemPropertyT : public RefCounted {
  BASE_TYPE(ItemPropertyT)

  virtual DataRef Evaluate(Item const&) const = 0;

  virtual String GetName() const = 0;

  FIELDS {}
};

template <class T>
Reference<T> ItemProperty_Create() {
  static Reference<T> value = new T;
  return value;
}

#define X(type, name, default)                                                 \
  struct ItemProperty_##name##T : public ItemPropertyT {                       \
    DERIVED_TYPE_EX(ItemProperty_##name##T)                                    \
                                                                               \
    LT_API DataRef Evaluate(Item const& item) const;                           \
                                                                               \
    type const& Get(Item const& item) {                                        \
      return item->Get##name();                                                \
    }                                                                          \
                                                                               \
    String GetName() const {                                                   \
      return #name;                                                            \
    }                                                                          \
  };                                                                           \
                                                                               \
  static Reference<ItemProperty_##name##T> ItemProperty_##name =               \
    ItemProperty_Create<ItemProperty_##name##T>();

ITEMPROPERTY_X
#undef X

#endif

#include "ItemProperty.h"

#include "Game/Capability.h"
#include "Game/Object.h"
#include "Game/Task.h"

#include "LTE/Renderable.h"

#include "UI/Icon.h"

#define X(type, name, default)                                                 \
  DERIVED_IMPLEMENT(ItemProperty_##name##T)                                    \
  DataRef ItemProperty_##name##T::Evaluate(Item const& item) const {           \
    return item->Get##name();                                                  \
  }
ITEMPROPERTY_X
#undef X

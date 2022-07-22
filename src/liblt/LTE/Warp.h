#ifndef LTE_WarpT_h__
#define LTE_WarpT_h__

#include "BaseType.h"
#include "Reference.h"

struct WarpT : public RefCounted {
  BASE_TYPE(WarpT)

  virtual V3 GetDelta(V3 const&) const = 0;
};

#endif

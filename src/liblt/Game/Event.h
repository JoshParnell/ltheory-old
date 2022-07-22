#ifndef Game_Event_h__
#define Game_Event_h__

#include "Common.h"
#include "LTE/BaseType.h"
#include "LTE/Reference.h"

struct EventT : public RefCounted {
  BASE_TYPE(EventT)

  FIELDS {}
};

#endif

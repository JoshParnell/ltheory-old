#include "../Events.h"

#include "LTE/Pool.h"

namespace {
  AutoClassDerived(EventMined, EventT, Event_Mined_Args, args)
    DERIVED_TYPE_EX(EventMined)
    POOLED_TYPE

    EventMined() {}
  };
}

DefineFunction(Event_Mined) {
  return new EventMined(args);
}

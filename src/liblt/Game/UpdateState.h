#ifndef UpdateState_h__
#define UpdateState_h__

#include "Common.h"

struct UpdateState {
  float dt;
  Time quanta;
  bool hasFocus;

  UpdateState(float dt, bool hasFocus = true) :
    dt(dt),
    quanta((Time)((float)kTimeScale * dt)),
    hasFocus(hasFocus)
    {}
};

#endif

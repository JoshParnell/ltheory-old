#ifndef Component_MotionControl_h__
#define Component_MotionControl_h__

#include "Common.h"
#include "Game/Item.h"
#include "LTE/AutoClass.h"
#include "LTE/SDF.h"
#include "LTE/Vector.h"

AutoClass(ComponentMotionControl,
  Vector<SDF>, elements)

  ComponentMotionControl() {}

  LT_API void Run(ObjectT* self, UpdateState& state);
};

AutoComponent(MotionControl)
  void OnUpdate(UpdateState& s) {
    MotionControl.Run(this, s);
    BaseT::OnUpdate(s);
  }
};

#endif

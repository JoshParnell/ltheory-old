#ifndef Component_ProximityTracker_h__
#define Component_ProximityTracker_h__

#include "Common.h"
#include "LTE/Vector.h"
#include "Game/Object.h"

AutoClass(ComponentProximityTracker,
  Vector<Object>, nearby,
  float, maxDistance,
  int, frameCounter)

  LT_API ComponentProximityTracker();

  LT_API bool IsNearby(ObjectT* self);
  LT_API void Run(ObjectT* self, UpdateState& state);
};

AutoComponent(ProximityTracker)
  void OnUpdate(UpdateState& s) {
    ProximityTracker.Run(this, s);
    BaseT::OnUpdate(s);
  }
};

#endif

#ifndef Component_Cullable_h__
#define Component_Cullable_h__

#include "Common.h"
#include "Game/Object.h"
#include "LTE/AutoClass.h"

AutoClass(ComponentCullable,
  float, cullDistanceSquared)

  ComponentCullable() : cullDistanceSquared(0.0f) {}

  void Recompute(ObjectT const* self) const {
    Mutable(this)->cullDistanceSquared =
      500 * self->GetRadius() * self->GetCullDistanceMult();
    Mutable(this)->cullDistanceSquared *= cullDistanceSquared;
  }
};

AutoComponent(Cullable)
};

#endif

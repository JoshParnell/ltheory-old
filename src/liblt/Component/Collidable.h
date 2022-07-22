#ifndef Component_Collidable_h__
#define Component_Collidable_h__

#include "Common.h"
#include "LTE/AutoClass.h"

AutoClass(ComponentCollidable,
  bool, passive,
  bool, solid)

  LT_API ComponentCollidable();

  LT_API void CheckCollisions(ObjectT* self, UpdateState& state);

  LT_API void Collide(
    ObjectT* self,
    ObjectT* other,
    Position const& pSelf,
    Position const& pOther);

  void Run(ObjectT* self, UpdateState& state) {
    if (!passive)
      CheckCollisions(self, state);
  }
};

AutoComponent(Collidable)
  void OnUpdate(UpdateState& s) {
    Collidable.Run(this, s);
    BaseT::OnUpdate(s);
  }
};

#endif

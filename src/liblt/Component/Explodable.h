#ifndef Component_Explodable_h__
#define Component_Explodable_h__

#include "Common.h"
#include "LTE/AutoClass.h"

AutoClass(ComponentExplodable,
  bool, exploded,
  ExplosionType, explosionType)

  ComponentExplodable() :
    exploded(false),
    explosionType(ExplosionType_Fire)
    {}

  LT_API void Run(ObjectT* self, UpdateState& state);
};

AutoComponent(Explodable)
  void OnUpdate(UpdateState& s) {
    Explodable.Run(this, s);
    BaseT::OnUpdate(s);
  }
};

#endif

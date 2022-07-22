#ifndef Component_Seeded_h__
#define Component_Seeded_h__

#include "Common.h"
#include "LTE/AutoClass.h"

AutoClass(ComponentSeeded,
  uint, seed)

  ComponentSeeded() :
    seed(0)
    {}
};

AutoComponent(Seeded)
  uint GetSeed() const {
    return Seeded.seed;
  }
};

#endif

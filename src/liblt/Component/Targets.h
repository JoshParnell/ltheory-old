#ifndef Component_Targets_h__
#define Component_Targets_h__

#include "Common.h"
#include "Game/Object.h"
#include "LTE/AutoClass.h"
#include "LTE/Vector.h"

AutoClass(ComponentTargets,
  Vector<Object>, elements)

  ComponentTargets() {}
};

AutoComponent(Targets)
};

#endif

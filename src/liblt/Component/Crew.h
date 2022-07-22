#ifndef Component_Crew_h__
#define Component_Crew_h__

#include "Common.h"
#include "Game/Item.h"
#include "LTE/AutoClass.h"
#include "LTE/Vector.h"

AutoClass(ComponentCrew,
  Vector<Item>, elements)

  ComponentCrew() {}
};

AutoComponent(Crew)
};

#endif
